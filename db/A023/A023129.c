/*
 * A023129  Signature sequence of sqrt(e).
 * https://oeis.org/A023129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A023129: signature sequence of sqrt(e).
   The algorithm merges the n arithmetic progressions i + j*sqrt(e)
   for j = 1 … n using a min‑heap keyed by the double value
   i + j*sqrt(e).  For each extraction the i‑coordinate is returned
   as a(n).  This works for all n ≥ 1. */

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    double val;   /* current value i + j*sqrt(e)      */
    idx_t  i;     /* current i for this j            */
    idx_t  j;     /* fixed j for this progression    */
} Node;

/* pre‑computed sqrt(e) as a double (used only for ordering). */
static const double SQRT_E = 1.648721270700128146848650787e0;

static void heapify_down(Node *h, size_t sz, size_t idx)
{
    while (1) {
        size_t l = idx * 2 + 1;
        size_t r = l + 1;
        size_t m = idx;
        if (l < sz && h[l].val < h[m].val) m = l;
        if (r < sz && h[r].val < h[m].val) m = r;
        if (m == idx) break;
        Node tmp = h[idx];
        h[idx] = h[m];
        h[m] = tmp;
        idx = m;
    }
}

static void heapify_up(Node *h, size_t sz, size_t idx)
{
    while (idx > 0) {
        size_t p = (idx - 1) >> 1;
        if (h[p].val <= h[idx].val) break;
        Node tmp = h[p];
        h[p] = h[idx];
        h[idx] = tmp;
        idx = p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety for offset=1 */

    size_t N = (size_t)n;
    Node *heap = (Node *)malloc(N * sizeof(Node));
    if (!heap) return (seq_t)0;              /* allocation failed */

    size_t sz = 0;
    /* initialise heap with first entry (i=1) for each j=1..n */
    for (idx_t j = 1; j <= n; ++j, ++sz) {
        heap[sz].i   = 1;
        heap[sz].j   = j;
        heap[sz].val = 1.0 + (double)j * SQRT_E;
    }

    /* build min‑heap */
    for (size_t i = sz / 2; i-- > 0;) heapify_down(heap, sz, i);

    seq_t result = 0;
    for (size_t count = 0; count < N; ++count) {
        /* extract smallest element */
        Node top = heap[0];
        result = (seq_t)top.i;

        /* replace root with last element */
        heap[0] = heap[--sz];
        if (sz > 0) heapify_down(heap, sz, 0);

        /* insert next element from the same j */
        top.i += 1;
        top.val += 1.0;           /* increment i by 1 adds 1.0 */
        heap[sz++] = top;
        heapify_up(heap, sz, sz - 1);
    }

    free(heap);
    return result;
}
