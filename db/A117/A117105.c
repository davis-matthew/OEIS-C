/*
 * A117105  Numbers that are the sum of three positive heptagonal numbers
 *          (A000566) in at least one way.
 * https://oeis.org/A117105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)/2
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_qsort_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117105: Numbers that are the sum of three positive heptagonal
   numbers (A000566) in at least one way.  The algorithm generates all
   sums H(i)+H(j)+H(k) in increasing order by merging m sorted lists, where
   each list corresponds to adding a fixed H(k) to all two‑sums
   H(i)+H(j).  The first n distinct sums are produced by a binary heap.
   The routine is tolerant to repeated calls with arbitrary n. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_qsort_cmp.h"

/* Allocators that work for both C and C++ */
#ifdef __cplusplus
#define NEW_ARRAY(type,count)  new type[count]
#define FREE_ARRAY(ptr)        delete[] ptr
#else
#define NEW_ARRAY(type,count)  ((type*)malloc(sizeof(type)*(count)))
#define FREE_ARRAY(ptr)        free(ptr)
#endif

/* Heap node for the merge algorithm */
typedef struct {
    seq_t  val;   /* current sum */
    int    list;  /* which H(k) is added (0‑based) */
    int    pos;   /* position in array2 (0‑based) */
} heap_node;

/* Simple binary min‑heap for heap_node, comparing by val */
static void heap_push(heap_node *heap, size_t *size, seq_t val,
                      int list, int pos)
{
    heap[*size].val  = val;
    heap[*size].list = list;
    heap[*size].pos  = pos;
    size_t i = *size;
    (*size)++;
    /* sift up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_node tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

static heap_node heap_pop(heap_node *heap, size_t *size)
{
    heap_node top = heap[0];
    (*size)--;
    if (*size > 0) {
        heap[0] = heap[*size];
        /* sift down */
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            if (l >= *size) break;
            size_t r = l + 1;
            size_t best = l;
            if (r < *size && heap[r].val < heap[l].val) best = r;
            if (heap[i].val <= heap[best].val) break;
            heap_node tmp = heap[i];
            heap[i] = heap[best];
            heap[best] = tmp;
            i = best;
        }
    }
    return top;
}

/* Compute the n-th term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe return */

    /* Start with a moderate number of heptagonal indices */
    idx_t m = 32;
    while (1) {
        /* Heptagonal numbers H[1..m] */
        seq_t *H = NEW_ARRAY(seq_t, m + 1);   /* 1‑based indexing */
        for (idx_t i = 1; i <= m; ++i) {
            seq_t ni = (seq_t)i;
            H[i] = ni * ((seq_t) 5 * ni - (seq_t)3) / (seq_t)2;
        }

        /* All pairwise sums H[i]+H[j] */
        size_t pair_len = (size_t)m * (size_t)m;
        seq_t *pair = NEW_ARRAY(seq_t, pair_len);
        size_t p = 0;
        for (idx_t i = 1; i <= m; ++i) {
            for (idx_t j = 1; j <= m; ++j) {
                pair[p++] = H[i] + H[j];
            }
        }
        qsort(pair, pair_len, sizeof(seq_t), seq_qsort_cmp);

        /* Min‑heap, one entry per list (fixed H[k]) */
        heap_node *heap = NEW_ARRAY(heap_node, m);
        size_t heap_sz = 0;
        for (int k = 0; k < (int)m; ++k) {
            heap_push(heap, &heap_sz, pair[0] + H[k + 1], k, 0);
        }

        /* Generate distinct sums in order until we reach the n‑th one */
        idx_t count = 0;
        bool last_set = false;
        seq_t last_val = 0;
        seq_t result = 0;

        while (count < n && heap_sz > 0) {
            heap_node cur = heap_pop(heap, &heap_sz);
            if (!last_set || cur.val != last_val) {
                ++count;
                last_val = cur.val;
                last_set = true;
                if (count == n) {
                    result = cur.val;
                    break;
                }
            }
            /* Push next value from the same list, if any */
            if ((size_t)(cur.pos + 1) < pair_len) {
                seq_t new_val = pair[cur.pos + 1] + H[cur.list + 1];
                heap_push(heap, &heap_sz, new_val, cur.list, cur.pos + 1);
            }
        }

        /* Free temporary structures */
        FREE_ARRAY(H);
        FREE_ARRAY(pair);
        FREE_ARRAY(heap);

        if (count >= n) {
            return result;
        }
        /* Not enough values – double the size and retry */
        m <<= 1;
    }
}
