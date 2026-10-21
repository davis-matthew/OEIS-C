/*
 * A215592  Value of y in the n-th number of the form x+y*(3+sqrt(13))/2.
 * https://oeis.org/A215592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y of the n‑th smallest number of the form x +
 *            y*(3+sqrt(13))/2
 * Range:     u8 n <= 584, u16 n <= 584, u32 n <= 584, u64 n <= 584,
 *            u128 n <= 584, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* heap node: current value A = 2*x + 3*y, and its y component B = y */
typedef struct {
    uint64_t A;   /* 2*x + 3*y   */
    uint64_t B;   /* y           */
} Entry;

/* return true if e1 represents a smaller number than e2 */
static int less_entry(const Entry *e1, const Entry *e2)
{
    int64_t u = (int64_t)e1->A - (int64_t)e2->A;
    int64_t v = (int64_t)e1->B - (int64_t)e2->B;

    if (v == 0)
        return u < 0;

    if (v > 0) {
        if (u < 0) {
            /* compare u^2 and 13*v^2 */
            __int128 lhs = (__int128)u * (__int128)u;
            __int128 rhs = (__int128)13 * (__int128)v * (__int128)v;
            return lhs > rhs;               /* u + v*sqrt13 < 0  */
        } else {
            return 0;                       /* u >= 0 so e1 > e2 */
        }
    } else { /* v < 0 */
        if (u >= 0) {
            __int128 lhs = (__int128)u * (__int128)u;
            __int128 rhs = (__int128)13 * (__int128)(-v) * (__int128)(-v);
            return lhs < rhs;               /* u + v*sqrt13 < 0  */
        } else {
            return 1;                       /* u < 0 so e1 < e2 */
        }
    }
}

/* percolate down the element at index 'i' in heap of size 'n' */
static void sift_down(Entry *heap, size_t i, size_t n)
{
    size_t root = i;
    while (1) {
        size_t left = 2 * root + 1;
        if (left >= n)
            break;
        size_t smallest = left;
        size_t right = left + 1;
        if (right < n && less_entry(&heap[right], &heap[smallest]))
            smallest = right;
        if (less_entry(&heap[smallest], &heap[root])) {
            Entry tmp = heap[root];
            heap[root] = heap[smallest];
            heap[smallest] = tmp;
            root = smallest;
        } else
            break;
    }
}

seq_t a(idx_t n)
{
    /* we need at least (n+1) heap nodes to keep enough sequences */
    size_t heap_size = (size_t)n + 1;
    Entry *heap = (Entry *)malloc(heap_size * sizeof(Entry));
    if (!heap)
        return (seq_t)0; /* allocation failed, return 0 as a safe value */

    /* initialise heap: one sequence for each y = 0..n */
    for (size_t y = 0; y <= n; ++y) {
        heap[y].A = 3ULL * y;   /* when x = 0 : 2*x + 3*y = 3*y */
        heap[y].B = y;
    }

    /* heapify */
    for (size_t i = heap_size / 2; i > 0; --i)
        sift_down(heap, i - 1, heap_size);

    /* extract the n-th element */
    for (idx_t i = 1; i <= n; ++i) {
        Entry e = heap[0];
        if (i == n) {
            free(heap);
            return (seq_t)e.B;
        }
        /* increment x for this sequence: A += 2 */
        heap[0].A += 2;
        sift_down(heap, 0, heap_size);
    }

    /* should never reach here */
    free(heap);
    return (seq_t)0;
}
