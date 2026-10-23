/*
 * A023118  Signature sequence of 1/sqrt(5) (arrange the numbers i+j*x (i,j >=
 *          1) in increasing order; the sequence of i's is the signature of
 *          x).
 * https://oeis.org/A023118
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

/* Signature sequence of 1/√5 computed by merging the infinite
   ascending lists for each fixed i with a min‑heap.
   Comparison is performed with exact integer arithmetic to avoid
   floating‑point operations. */

#include <stdlib.h>
#include <stddef.h>

/* Node holding the pair (i , j).  i, j ≥ 1. */
typedef struct {
    idx_t i;
    idx_t j;
} node_t;

/* Return true if node a < node b in the ordering defined by
   a.i + a.j/√5 < b.i + b.j/√5.  The comparison never uses √5
   explicitly. */
static inline int node_less(const node_t *a, const node_t *b)
{
    if (a->i == b->i)        /* same i → smaller j comes first   */
        return a->j < b->j;
    if (a->j == b->j)        /* same j → smaller i comes first   */
        return a->i < b->i;

    /* Use the algebraic test derived from
       (i1 + j1/√5) < (i2 + j2/√5)  ↔ (i1−i2)*√5 < (j2−j1). */
    int64_t di = (int64_t)a->i - (int64_t)b->i;          /* Δi = i1−i2 */
    int64_t dj = (int64_t)b->j - (int64_t)a->j;          /* Δj = j2−j1 */

    if (di > 0) {                /* Δi positive  */
        if (dj <= 0)             /* RHS non‑positive → a > b */
            return 0;
        /* both sides positive → compare squares */
        __int128 left  = (__int128)di * di * 5;   /* Δi²·5   */
        __int128 right = (__int128)dj * dj;       /* Δj²     */
        return left < right;
    }

    if (di < 0) {                /* Δi negative */
        if (dj >= 0)             /* RHS ≥ 0   → a < b */
            return 1;
        /* both sides negative → compare squares in reverse order */
        __int128 left  = (__int128)(-di) * (-di) * 5; /* (-Δi)²·5 */
        __int128 right = (__int128)(-dj) * (-dj);     /* (-Δj)²   */
        return left > right;
    }

    return 0;   /* di == 0 and dj == 0 – equal nodes (never occurs) */
}

static inline void swap_node(node_t *x, node_t *y)
{
    node_t t = *x;
    *x = *y;
    *y = t;
}

static void heapify_up(node_t *h, size_t len, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (!node_less(&h[parent], &h[idx]))
            break;
        swap_node(&h[parent], &h[idx]);
        idx = parent;
    }
}

static void heapify_down(node_t *h, size_t len, size_t idx)
{
    while (1) {
        size_t left = idx * 2 + 1;
        if (left >= len)
            break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < len && node_less(&h[right], &h[left]))
            smallest = right;
        if (!node_less(&h[smallest], &h[idx]))
            break;
        swap_node(&h[smallest], &h[idx]);
        idx = smallest;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* offset is 1 – not used */

    /* The first n elements can involve at most n different i values,
       so a heap of size n suffices. */
    size_t heap_len = (size_t)n;
    node_t *heap = (node_t*)malloc(heap_len * sizeof(node_t));
    if (!heap)
        return 0;                   /* out of memory guard */

    /* Initialize heap with j = 1 for each i = 1 .. n. */
    for (idx_t i = 1; i <= n; ++i) {
        heap[i - 1].i = i;
        heap[i - 1].j = 1;
    }

    /* Build the initial min‑heap. */
    for (size_t k = heap_len / 2; k > 0; --k)
        heapify_down(heap, heap_len, k - 1);

    seq_t result = 0;

    for (size_t step = 0; step < n; ++step) {
        /* Extract current minimum (node at root). */
        result = (seq_t)heap[0].i;

        /* Advance the j‑index for the extracted i and re‑heapify. */
        heap[0].j += 1;
        heapify_down(heap, heap_len, 0);
    }

    free(heap);
    return result;
}
