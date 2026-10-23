/*
 * A048407  Numbers whose consecutive digits differ by 5.
 * https://oeis.org/A048407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 34, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A048407: numbers whose consecutive digits differ by 5 */
#include <stdlib.h>
#include <stdbool.h>

/* Returns the nth term of the sequence A048407.
   The sequence is defined as all non‑negative integers whose
   consecutive decimal digits differ by exactly 5, sorted in
   increasing order, with a(0)=0. */
seq_t a(idx_t n) {
    /* Local heap implementation for generating terms in ascending order. */
    static seq_t *heap = NULL;        /* priority queue contents */
    static size_t heap_sz = 0;        /* current number of elements in heap */
    static size_t heap_cap = 0;       /* allocated capacity of heap */

    /* Cache of generated sequence terms. */
    static seq_t *seq = NULL;        /* array of terms */
    static size_t seq_len = 0;       /* number of terms stored */

    /* One‑time initialization. */
    if (seq_len == 0) {
        /* Store a(0)=0. */
        seq_len = 1;
        seq = (seq_t *)malloc(sizeof(seq_t));
        seq[0] = (seq_t)0;

        /* Insert single‑digit numbers 1..9 into the heap. */
        for (int d = 1; d <= 9; ++d) {
            /* Grow the heap if necessary. */
            if (heap_sz == heap_cap) {
                heap_cap = heap_cap ? heap_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(heap, heap_cap * sizeof(seq_t));
                heap = tmp;
            }
            heap[heap_sz++] = (seq_t)d;
        }
    }

    /* Generate terms until we have at least n+1 terms in the cache. */
    while (seq_len <= n) {
        /* Extract the minimum element from the heap. */
        seq_t cur = heap[0];
        --heap_sz;
        if (heap_sz > 0) heap[0] = heap[heap_sz];
        /* Sift down to restore heap property. */
        size_t i = 0;
        while (true) {
            size_t l = (i << 1) + 1;
            if (l >= heap_sz) break;
            size_t r = l + 1;
            size_t child = (r < heap_sz && heap[r] < heap[l]) ? r : l;
            if (heap[i] <= heap[child]) break;
            seq_t tmp = heap[i]; heap[i] = heap[child]; heap[child] = tmp;
            i = child;
        }

        /* Store the extracted term. */
        seq = (seq_t *)realloc(seq, (seq_len + 1) * sizeof(seq_t));
        seq[seq_len++] = cur;

        /* Generate successors: append digits differing by 5. */
        seq_t last = cur % (seq_t)10;
        if (last < (seq_t)5) {
            seq_t nxt = cur * (seq_t)10 + (last + (seq_t)5);
            /* Grow heap if needed. */
            if (heap_sz == heap_cap) {
                heap_cap = heap_cap ? heap_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(heap, heap_cap * sizeof(seq_t));
                heap = tmp;
            }
            /* Insert new element and sift up. */
            size_t pos = heap_sz++;
            heap[pos] = nxt;
            while (pos > 0) {
                size_t parent = (pos - 1) >> 1;
                if (heap[parent] <= heap[pos]) break;
                seq_t tmp = heap[parent]; heap[parent] = heap[pos]; heap[pos] = tmp;
                pos = parent;
            }
        }
        if (last >= (seq_t)5) {
            seq_t nxt = cur * (seq_t)10 + (last - (seq_t)5);
            if (heap_sz == heap_cap) {
                heap_cap = heap_cap ? heap_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(heap, heap_cap * sizeof(seq_t));
                heap = tmp;
            }
            size_t pos = heap_sz++;
            heap[pos] = nxt;
            while (pos > 0) {
                size_t parent = (pos - 1) >> 1;
                if (heap[parent] <= heap[pos]) break;
                seq_t tmp = heap[parent]; heap[parent] = heap[pos]; heap[pos] = tmp;
                pos = parent;
            }
        }
    }

    return seq[n];
}
