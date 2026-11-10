/*
 * A080784  Duplicate of A072978.
 * https://oeis.org/A080784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th smallest number of the form m*2^bigomega(m), m odd
 *            (m=1 gives 1); computed by a search with a bounded max-heap
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 22, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-07)
 * Helpers:   bigomega, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/mem.h"

static void A080784_sift_down(seq_t *h, size_t size, size_t i)
{
    for (;;) {
        size_t l = i << 1;
        if (l > size) break;
        size_t r = l + 1;
        size_t child = l;
        if (r <= size && h[r] > h[l]) child = r;
        if (h[i] >= h[child]) break;
        seq_t t = h[i]; h[i] = h[child]; h[child] = t;
        i = child;
    }
}

static void A080784_sift_up(seq_t *h, size_t i)
{
    while (i > 1) {
        size_t p = i >> 1;
        if (h[p] >= h[i]) break;
        seq_t t = h[p]; h[p] = h[i]; h[i] = t;
        i = p;
    }
}

seq_t a(idx_t n)
{
    seq_t *heap = mem_new(seq_t, (n + 1));
    size_t size = 0;

    for (idx_t m = 1; ; m += 2) {
        unsigned int omega = (unsigned int)bigomega(m);
        seq_t val = ((seq_t)m) << omega;

        if (size < n) {
            heap[++size] = val;
            A080784_sift_up(heap, size);
        } else if (val < heap[1]) {
            heap[1] = val;
            A080784_sift_down(heap, size, 1);
        }

        if (size == n) {
            seq_t next_min = (((seq_t)(m + 2)) << 1);
            if (next_min > heap[1]) {
                seq_t result = heap[1];
                mem_free(heap);
                return result;
            }
        }
    }
}
