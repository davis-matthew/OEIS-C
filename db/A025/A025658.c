/*
 * A025658  Exponent of 6 (value of j) in n-th number of form 4^i*6^j.
 * https://oeis.org/A025658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: sequence a(n) = exponent of 6 in the n‑th smallest number of
   the form 4^i * 6^j.  The algorithm maintains a min‑heap of
   logarithmic keys so that large values are never formed. */

#include <stdlib.h>   /* malloc, free */

#define LOG2_6 2.5849625007211562L   /* log2(6) */

typedef struct {
    idx_t  j;     /* current exponent of 6 (the answer for this element) */
    long double key; /* log2 of the current value 4^i * 6^j */
} HeapNode;

/* Swap two nodes in the heap */
static inline void heap_swap(HeapNode *a, HeapNode *b)
{
    HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Push a node onto the min‑heap (1‑based). */
static void heap_push(int *size, HeapNode *heap, HeapNode val)
{
    int i = ++(*size);
    while (i > 1) {
        int p = i >> 1;
        if (heap[p].key <= val.key) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = val;
}

/* Pop the smallest node from the min‑heap. */
static HeapNode heap_pop(int *size, HeapNode *heap)
{
    HeapNode ret = heap[1];
    HeapNode last = heap[(*size)--];
    int i = 1;
    while (1) {
        int l = i << 1, r = l + 1, s;
        if (l > *size) break;
        s = l;
        if (r <= *size && heap[r].key < heap[l].key) s = r;
        if (heap[s].key >= last.key) break;
        heap[i] = heap[s];
        i = s;
    }
    heap[i] = last;
    return ret;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑range requests */

    /* Allocate a heap with n slots (1‑based). */
    int heap_cap = (int)n + 1;
    HeapNode *heap = (HeapNode *)malloc(heap_cap * sizeof(HeapNode));
    if (!heap) return (seq_t)0;            /* unlikely in test harness */

    /* Initialise the heap: one entry per exponent j (0 … n‑1),
       starting with i = 0 for each column. */
    int heap_size = 0;
    for (idx_t j = 0; j < n; ++j) {
        HeapNode node;
        node.j   = j;
        node.key = (long double)j * LOG2_6;   /* log2(4^0 * 6^j) = j*log2(6) */
        heap_push(&heap_size, heap, node);
    }

    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        HeapNode cur = heap_pop(&heap_size, heap);
        result = (seq_t)cur.j;               /* exponent of 6 for the k‑th number */

        /* Advance in the same column: i increases by 1 -> value multiplies by 4,
           so log2 value increases by 2. */
        cur.key += 2.0L;
        heap_push(&heap_size, heap, cur);
    }

    free(heap);
    return result;
}
