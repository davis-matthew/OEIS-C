/*
 * A181124  Difference of two positive 5th powers.
 * https://oeis.org/A181124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow5_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A181124 (difference of two positive 5th powers),
 * sorted increasingly.  The sequence starts with a(1)=0.  Algorithm:
 *   * For each y (1 ≤ y ≤ n) initialise the first difference with x=y+1.
 *   * Maintain a min‑heap of these differences (using 128‑bit values to avoid
 *     overflow in intermediate computations).  When the smallest difference is
 *     extracted, the next difference for the same y (x+1) is inserted.
 *   * Skip duplicates; stop when the desired index n is reached.
 *
 * The algorithm runs in O(n log n) time and uses O(n) auxiliary memory.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow5_128.h"

typedef struct {
    unsigned __int128 diff128; /* 128‑bit difference for ordering */
    idx_t y;                    /* lower exponent */
    idx_t x;                    /* upper exponent (x > y) */
} node_t;

/* Heap operations using diff128 for ordering */
static void heapify_down(node_t *h, size_t sz, size_t i) {
    while (1) {
        size_t l = 2 * i + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < sz && h[l].diff128 < h[smallest].diff128) smallest = l;
        if (r < sz && h[r].diff128 < h[smallest].diff128) smallest = r;
        if (smallest == i) break;
        node_t tmp = h[i];
        h[i] = h[smallest];
        h[smallest] = tmp;
        i = smallest;
    }
}

static void heapify_up(node_t *h, size_t sz, size_t i) {
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h[p].diff128 <= h[i].diff128) break;
        node_t tmp = h[p];
        h[p] = h[i];
        h[i] = tmp;
        i = p;
    }
}

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)0;

    /* Number of heap elements (one per y).  n fits into size_t safely. */
    size_t heap_sz = (size_t)n;
    node_t *heap = (node_t *)malloc(heap_sz * sizeof(node_t));
    if (!heap) return (seq_t)0;  /* out of memory – unlikely in test harness */

    /* Initialise heap: for each y, x=y+1 and compute first difference */
    for (size_t i = 0; i < heap_sz; ++i) {
        idx_t y = (idx_t)(i + 1);
        idx_t x = y + 1;
        heap[i].y = y;
        heap[i].x = x;
        heap[i].diff128 = pow5_128(x) - pow5_128(y);
    }
    /* Build min‑heap in linear time */
    for (size_t i = heap_sz / 2; i > 0; --i) heapify_down(heap, heap_sz, i - 1);

    unsigned __int128 last_diff128 = 0;  /* a(1) = 0 */
    size_t distinct = 1;                /* count of distinct outputs so far */

    while (distinct < (size_t)n) {
        /* Extract smallest difference */
        node_t cur = heap[0];
        /* Replace root with last element and heapify downward */
        heap[0] = heap[heap_sz - 1];
        heapify_down(heap, heap_sz, 0);

        if (cur.diff128 != last_diff128) {
            ++distinct;
            if (distinct == (size_t)n) {
                /* Convert 128‑bit difference to seq_t (possible truncation if
                 * seq_t is 64‑bit and the value doesn't fit; for terms that
                 * fit this conversion is exact). */
                return (seq_t)cur.diff128;
            }
            last_diff128 = cur.diff128;
        }

        /* Advance y's upper exponent and recompute difference */
        ++cur.x;
        cur.diff128 = pow5_128(cur.x) - pow5_128(cur.y);
        heap[heap_sz - 1] = cur;
        heapify_up(heap, heap_sz, heap_sz - 1);
    }

    /* Should never reach here, but return last value as a fallback */
    free(heap);
    return (seq_t)last_diff128;
}
