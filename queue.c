#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (!q)
        return NULL;
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    if (!q)
        return;
    list_ele_t *del;
    while (q->head) {
        del = q->head->next;
        free(q->head->value);
        free(q->head);
        q->head = del;
    }
    q->size = 0;
    /* Free queue structure */
    free(q);
    return;
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q)
        return false;
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    newh->next = q->head;
    q->head = newh;
    q->size++;
    if (!q->tail)
        q->tail = newh;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    newt->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newt->value) {
        free(newt);
        return false;
    }
    newt->next = NULL;
    strncpy(newt->value, s, strlen(s) + 1);
    if (!q->head) {
        q->head = newt;
        q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = newt;
    }
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;
    list_ele_t *rh;
    rh = q->head;
    q->head = q->head->next;
    q->size--;
    if (sp) {
        strncpy(sp, rh->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    free(rh->value);
    free(rh);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size == 1)
        return;
    list_ele_t *iter = q->head;
    list_ele_t *prev = NULL, *next = NULL;
    q->tail = q->head;
    while (iter != NULL) {
        next = iter;
        iter = iter->next;
        next->next = prev;
        prev = next;
    }
    q->head = next;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    // merge with pseudo node
    if (!l1)
        return l2;
    if (!l2)
        return l1;
    list_ele_t *tmp = NULL;
    list_ele_t *head = NULL;

    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) < 0) {
            if (!tmp) {
                head = l1;
                tmp = head;
                l1 = l1->next;
            } else {
                tmp->next = l1;
                l1 = l1->next;
                tmp = tmp->next;
            }
        } else {
            if (!tmp) {
                head = l2;
                tmp = head;
                l2 = l2->next;
            } else {
                tmp->next = l2;
                l2 = l2->next;
                tmp = tmp->next;
            }
        }
    }
    while (l1 || l2) {
        if (l1) {
            tmp->next = l1;
            l1 = l1->next;
        } else {
            tmp->next = l2;
            l2 = l2->next;
        }
        tmp = tmp->next;
    }
    return head;
}

list_ele_t *mergeSortList(list_ele_t *head)
{
    // merge sort
    if (!head || !head->next)
        return head;

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *l1 = mergeSortList(head);
    list_ele_t *l2 = mergeSortList(fast);

    // merge sorted l1 and sorted l2
    return merge(l1, l2);
}
void q_sort(queue_t *q)
{
    if (!q || q->size == 1 || !q->head)
        return;
    q->head = mergeSortList(q->head);
    list_ele_t *result = q->head;
    while (result->next) {
        result = result->next;
    }
    q->tail = result;
}
