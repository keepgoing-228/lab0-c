#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new = malloc(sizeof(struct list_head));
    if (!new)
        return NULL;
    INIT_LIST_HEAD(new);
    return new;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    // check if queue exist
    if (!head)
        return;

    struct list_head *current, *next;
    // current will be set to the current node in list_for_each_safe function
    list_for_each_safe(current, next, head) {
        // e point to the element_t that contain current list_head
        element_t *e = list_entry(current, element_t, list);
        free(e->value);
        free(e);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    // check if queue exists
    if (!head)
        return false;

    element_t *ele = malloc(sizeof(element_t));
    ele->value = strdup(s);
    // check if char exists
    if (!ele->value) {
        free(ele);
        return false;
    }

    list_add(&ele->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    // if (!head)
    //     return false;

    // element_t *ele = malloc(sizeof(element_t));
    // ele->value = strdup(s);
    // if (!ele->value) {
    //     free(ele);
    //     return false;
    // }

    // list_add_tail(&ele->list, head);
    // return true;

    // use q_insert_head to insert at tail directly
    return q_insert_head(head->prev, s);
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    // check if queue exists or is empty
    if (!head || list_empty(head))
        return NULL;

    struct list_head *first = head->next;
    element_t *ele = list_entry(first, element_t, list);

    // copy the string to sp
    if (sp) {
        strncpy(sp, ele->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    // remove the node from the list, not delete
    list_del(first);

    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    // if (!head || list_empty(head))
    //     return NULL;

    // struct list_head *last = head->prev;
    // element_t *ele = list_entry(last, element_t, list);

    // if (sp) {
    //     strncpy(sp, ele->value, bufsize - 1);
    //     sp[bufsize - 1] = '\0';
    // }

    // list_del(last);
    // return ele;

    // use q_remove_head to remove at tail directly
    return q_remove_head(head->prev->prev, sp, bufsize);
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each(li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head *slow = head->next;
    struct list_head *fast = head->next;

    while (fast != head &&
           fast->next != head) {  // while (fast && fast->next) is not work in
                                  // double liked list
        slow = slow->next;
        fast = fast->next->next;
    }

    list_del(slow);
    q_release_element(list_entry(slow, element_t, list));

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    // check if correct?

    if (!head || list_empty(head))
        return false;

    struct list_head *prev = head;
    struct list_head *cur = head->next;

    while (cur != head) {
        while (cur->next && list_entry(cur, element_t, list)->value ==
                                list_entry(cur->next, element_t, list)->value) {
            cur = cur->next;
        }

        // Check if the current node is a duplicate, if so, remove it
        if (prev->next != cur) {
            prev->next = cur->next;
            cur->next->prev = prev;
            cur = cur->next;
        } else {
            prev = cur;
            cur = cur->next;
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head))
        return;

    struct list_head *cur = head;

    while (cur->next != head && cur->next->next != head) {
        struct list_head *first = cur->next;
        struct list_head *second = first->next;

        list_move(second, cur);

        cur = first;  // skip the two nodes, not cur = second
    }

    // q_reverseK(head, 2);
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    q_reverseK(head, q_size(head));
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    int size = q_size(head);
    if (!head || list_empty(head) || k <= 1 || k > size)
        return;

    struct list_head *start = head;
    int remaining = size;

    while (remaining >= k) {
        // Process k nodes at a time
        struct list_head *cur = start;

        // Reverse k nodes
        for (int i = 0; i < k; i++) {
            struct list_head *tmp = cur->next;
            // Skip the first node in the group
            if (i > 0) {
                // Move the current node to the beginning of the group
                list_move(tmp, start);
            }
            cur = tmp;
        }

        // Move to the next group
        start = cur;
        remaining -= k;
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
