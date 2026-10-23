/*
 * A139507  Primes of the form x^2 + 27x*y + y^2 for x and y nonnegative.
 * https://oeis.org/A139507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Primes of the form x^2 + 27xy + y^2 with x,y ≥ 1.
 * The function builds the sequence by merging the increasing
 * sequences for each fixed x using a min‑heap.  Primality is
 * tested with is_prime_big() from the utils library.
 * Subsequent calls are served from a static cache of already
 * found primes.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

typedef struct {
    seq_t val;
    idx_t x, y;
} Node;

/* static cache of primes */
static seq_t *cache   = NULL;
static size_t cache_cap = 0, cache_size = 0;

/* min‑heap of active sequences (one entry per x) */
static Node *heap   = NULL;
static size_t heap_cap = 0, heap_size = 0;

/* the next x that has not yet been inserted into the heap */
/* x=1 is inserted during the first call */
static idx_t next_x = 2;

/* ------------------------------------------------------------------ */
/* evaluate the quadratic form for given x and y */
static seq_t f_val(idx_t x, idx_t y)
{
    seq_t sx = x;
    seq_t sy = y;
    return sx*sx + ((seq_t)27)*sx*sy + sy*sy;
}

/* ------------------------------------------------------------------ */
/* heap helper: swap two nodes */
static inline void heap_swap(Node *a, Node *b)
{
    Node t = *a;
    *a = *b;
    *b = t;
}

/* push a node onto the heap */
static void heap_push(Node node)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap*2 : 16;
        Node *tmp = (Node*)realloc(heap, heap_cap * sizeof(Node));
        heap = tmp;
    }
    heap[heap_size++] = node;
    size_t i = heap_size - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_swap(&heap[p], &heap[i]);
        i = p;
    }
}

/* pop the smallest node from the heap */
static Node heap_pop_min(void)
{
    Node top = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_size && heap[l].val < heap[smallest].val) smallest = l;
        if (r < heap_size && heap[r].val < heap[smallest].val) smallest = r;
        if (smallest == i) break;
        heap_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return top;
}

/* ------------------------------------------------------------------ */
/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, undefined for 0 */

    /* initialise cache on first use */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* initialise heap on first use */
    if (!heap) {
        heap_cap = 16;
        heap = (Node*)malloc(heap_cap * sizeof(Node));
    }

    /* generate terms until we have at least n primes */
    while (cache_size < (size_t)n) {
        /* seed heap with x=1 for the first iteration */
        if (heap_size == 0) {
            Node seed = { f_val(1, 1), 1, 1 };
            heap_push(seed);
        }

        /* insert new x values whose first term is no larger than
         * the current minimum.  This keeps the heap compact.
         */
        while (true) {
            seq_t start_val = f_val(next_x, 1);
            if (heap[0].val < start_val) break;
            Node newnode = { start_val, next_x, 1 };
            heap_push(newnode);
            ++next_x;
        }

        /* extract the smallest value */
        Node cur = heap_pop_min();

        /* store it if it is a new prime */
        if (! (cache_size > 0 && cur.val == cache[cache_size-1]) &&
            is_prime_big(cur.val))
        {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = cur.val;
        }

        /* advance y for this x and push the new term */
        ++cur.y;
        cur.val = f_val(cur.x, cur.y);
        heap_push(cur);
    }

    return cache[n-1];
}
