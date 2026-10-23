/*
 * A025678  Exponent of 9 (value of j) in n-th number of form 2^i*9^j.
 * https://oeis.org/A025678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A025678 – Exponent of 9 in the n‑th smallest integer of the form
    2^i * 9^j.
    The implementation keeps a min‑heap of pairs (i,j) where the key is
    i + j*log2(9).  When the smallest element is removed, i is increased
    by 1 (equivalent to multiplying the value by 2) and the node is
    re‑inserted.  After n removals the j of the last removed node is
    returned.  This runs in O(n log n) time and uses only O(n) memory. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    idx_t   i;            /* exponent of 2   */
    idx_t   j;            /* exponent of 9   */
    long double key;      /* ordering key    */
} heap_node_t;

/* percolate a node down to restore the min‑heap property */
static void sift_down(heap_node_t *heap, size_t idx, size_t size)
{
    while (1) {
        size_t left  = 2 * idx + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left < size && heap[left].key < heap[smallest].key)
            smallest = left;
        if (right < size && heap[right].key < heap[smallest].key)
            smallest = right;

        if (smallest == idx)
            break;

        /* swap */
        heap_node_t tmp   = heap[idx];
        heap[idx]          = heap[smallest];
        heap[smallest]     = tmp;
        idx               = smallest;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based.  For n == 0 we return 0 as a safe default. */
    if (n == 0)
        return (seq_t)0;

    /* number of nodes in the heap – one for each possible j up to n-1 */
    size_t heap_size = (size_t)n;
    heap_node_t *heap = (heap_node_t *)malloc(heap_size * sizeof(heap_node_t));
    if (!heap)
        return (seq_t)0;            /* out‑of‑memory fall back */

    static const long double LOG2_9 = log2l(9.0L);

    /* initialise the heap with i = 0 for all j */
    for (size_t j = 0; j < heap_size; ++j) {
        heap[j].i   = 0;
        heap[j].j   = j;
        heap[j].key = (long double)j * LOG2_9;
    }

    /* build the heap */
    for (ptrdiff_t i = (ptrdiff_t)heap_size / 2 - 1; i >= 0; --i)
        sift_down(heap, (size_t)i, heap_size);

    /* repeatedly extract the smallest and advance its i */
    for (size_t step = 1; step <= heap_size; ++step) {
        heap_node_t *top = &heap[0];
        if (step == heap_size) {
            seq_t result = (seq_t)top->j;
            free(heap);
            return result;
        }

        /* advance i (multiply by 2) */
        ++top->i;
        top->key += 1.0L;            /* log2(2) == 1 */
        sift_down(heap, 0, heap_size);
    }

    /* should never reach here */
    free(heap);
    return (seq_t)0;
}
