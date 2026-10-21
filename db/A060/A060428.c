/*
 * A060428  Numbers of form x^2 + xy + y^2 (with repetitions if more than one
 *          representation is possible).
 * https://oeis.org/A060428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   less, val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers of the form x^2 + x*y + y^2 with
   0 ≤ y ≤ x and each unordered pair counted separately,
   sorted in nondecreasing order. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/less.h"
#include "../utils/val.h"

typedef struct {
    seq_t value; /* value of x^2 + x*y + y^2 */
    idx_t x;     /* current x */
    idx_t y;     /* current y */
} node_t;

static node_t *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_sz  = 0;

/* Ensure the heap has room for at least 'n' elements. */
static void heap_ensure(size_t n)
{
    if (heap_sz + n <= heap_cap)
        return;
    size_t new_cap = heap_cap ? heap_cap * 2 : 16;
    while (new_cap < heap_sz + n)
        new_cap *= 2;
    heap = (node_t *)realloc(heap, new_cap * sizeof(node_t));
    heap_cap = new_cap;
}

/* Swap two heap nodes. */
static void heap_swap(size_t i, size_t j)
{
    node_t tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

/* Push a node onto the heap. */
static void heap_push(node_t node)
{
    heap_ensure(1);
    size_t i = heap_sz++;
    heap[i] = node;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (!less(heap[i].value, heap[p].value))
            break;
        heap_swap(i, p);
        i = p;
    }
}

/* Pop the minimum node from the heap. */
static node_t heap_pop(void)
{
    node_t res = heap[0];
    heap[0] = heap[--heap_sz];
    size_t i = 0;
    for (;;) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t best = i;
        if (l < heap_sz && less(heap[l].value, heap[best].value))
            best = l;
        if (r < heap_sz && less(heap[r].value, heap[best].value))
            best = r;
        if (best == i)
            break;
        heap_swap(i, best);
        i = best;
    }
    return res;
}

/* The required function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Initialize heap with the first pair (1,0). */
    heap_sz = 0;
    heap_cap = 0;
    heap_ensure(16);

    idx_t next_x = 2; /* Next x to push when we pop a y == 0 pair. */
    heap_push((node_t){ .value = val(1, 0), .x = 1, .y = 0 });

    idx_t count = 0;
    while (1) {
        node_t cur = heap_pop();
        count++;
        if (count == n)
            return cur.value;

        /* Push the pair with the next y if still within this x. */
        if (cur.y < cur.x) {
            heap_push((node_t){
                .value = val(cur.x, cur.y + 1),
                .x = cur.x,
                .y = cur.y + 1
            });
        }

        /* When the smallest y (i.e., y==0) for this x has been popped,
           add the first pair for the next x (unless it is out of range). */
        if (cur.y == 0) {
            heap_push((node_t){
                .value = val(next_x, 0),
                .x = next_x,
                .y = 0
            });
            next_x++;
        }
    }
}
