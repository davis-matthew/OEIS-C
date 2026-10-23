/*
 * A025680  Exponent of 9 (value of j) in n-th number of form 6^i*9^j.
 * https://oeis.org/A025680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   visited_add, visited_has
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A025680 – exponent of 9 in the n‑th number of the form 6^i · 9^j.
 * The numbers 6^i·9^j are generated in increasing order by a min‑heap.
 * The function returns the exponent j of the n‑th such number (1‑based). */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t, uint32_t */
#include <stdlib.h>      /* malloc, free, calloc, memset */
#include <math.h>        /* log */
#include "../utils/visited_add.h"
#include "../utils/visited_has.h"

#define VISITED_CAP (1U << 18)  /* 262144 – enough for n up to thousands */

/* heap entry: exponents (i,j) and the logarithm of 6^i·9^j */
typedef struct {
    uint32_t i, j;
    double  logv;
} node;

/* in‑heap comparison by logv (smaller value first) */
static inline int node_cmp(const node *a, const node *b)
{
    return (a->logv < b->logv) ? -1 : (a->logv > b->logv) ? 1 : 0;
}

/* move node at pos up the heap */
static void heap_bubble_up(node *h, size_t pos)
{
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (node_cmp(&h[pos], &h[parent]) < 0) {
            node tmp = h[pos];
            h[pos] = h[parent];
            h[parent] = tmp;
            pos = parent;
        } else break;
    }
}

/* move node at pos down the heap */
static void heap_bubble_down(node *h, size_t sz, size_t pos)
{
    while (1) {
        size_t left = (pos << 1) | 1;
        if (left >= sz) break;
        size_t right = left + 1;
        size_t child = (right < sz && node_cmp(&h[right], &h[left]) < 0) ? right : left;
        if (node_cmp(&h[child], &h[pos]) < 0) {
            node tmp = h[pos];
            h[pos] = h[child];
            h[child] = tmp;
            pos = child;
        } else break;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* outside the offset */

    /* hash table for visited (i,j) pairs */
    uint64_t *visited = (uint64_t *)calloc(VISITED_CAP, sizeof(uint64_t));
    if (!visited) return (seq_t)0;        /* allocation failed */

    /* heap capacity – at most about 2*n + 10 entries are needed */
    size_t heap_cap = 2 * n + 10;
    node *heap = (node *)malloc(heap_cap * sizeof(node));
    if (!heap) { free(visited); return (seq_t)0; }

    /* logarithms of 6 and 9 (constant time) */
    const double LOG6 = log(6.0);
    const double LOG9 = log(9.0);

    /* initial entry: i=0, j=0, value=1 */
    heap[0].i = 0;
    heap[0].j = 0;
    heap[0].logv = 0.0;
    size_t heap_sz = 1;

    idx_t popped = 0;                     /* numbers already output */

    while (popped < n) {
        /* pop smallest */
        node cur = heap[0];
        heap[0] = heap[--heap_sz];
        heap_bubble_down(heap, heap_sz, 0);

        ++popped;
        if (popped == n) {               /* we found the n-th number */
            seq_t result = (seq_t)cur.j;
            free(heap);
            free(visited);
            return result;
        }

        /* generate child (i+1, j) – multiply by 6 */
        uint32_t ni = cur.i + 1;
        uint32_t nj = cur.j;
        uint64_t key1 = ((uint64_t)ni << 32) | nj;
        if (!visited_has(key1, visited, VISITED_CAP)) {
            visited_add(key1, visited, VISITED_CAP);
            if (heap_sz < heap_cap) {
                heap[heap_sz].i = ni;
                heap[heap_sz].j = nj;
                heap[heap_sz].logv = cur.logv + LOG6;
                heap_bubble_up(heap, heap_sz);
                ++heap_sz;
            }
        }

        /* generate child (i, j+1) – multiply by 9 */
        ni = cur.i;
        nj = cur.j + 1;
        uint64_t key2 = ((uint64_t)ni << 32) | nj;
        if (!visited_has(key2, visited, VISITED_CAP)) {
            visited_add(key2, visited, VISITED_CAP);
            if (heap_sz < heap_cap) {
                heap[heap_sz].i = ni;
                heap[heap_sz].j = nj;
                heap[heap_sz].logv = cur.logv + LOG9;
                heap_bubble_up(heap, heap_sz);
                ++heap_sz;
            }
        }
    }

    free(heap);
    free(visited);
    return (seq_t)0; /* unreachable */
}
