/*
 * A072978  Numbers of the form m * 2^bigomega(m), where m>1 is odd and
 *          bigomega(m) = A001222(m), the number of prime factors of m.
 * https://oeis.org/A072978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 854, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, sift_down, sift_up
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of the form m * 2^bigomega(m) with m > 1 odd; a(1) = 1. */

#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/sift_down.h"
#include "../utils/sift_up.h"

seq_t a(idx_t n)
{
    /* Allocate a binary max‑heap of capacity n+1. */
    seq_t *heap = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    size_t size = 0;

    for (idx_t m = 1; ; m += 2) {
        unsigned int omega = (unsigned int)bigomega(m);
        seq_t val = ((seq_t)m) << omega;          /* m * 2^Ω(m) */

        if (size < n) {
            heap[++size] = val;
            sift_up(heap, size);
        } else if (val < heap[1]) {
            heap[1] = val;
            sift_down(heap, size, 1);
        }

        if (size == n) {
            /* The smallest possible value from any future odd m is
               (m + 2) * 2.  If that exceeds the current maximum in the
               heap, no later value can be smaller than the current nth
               term. */
            seq_t next_min = (((seq_t)(m + 2)) << 1);
            if (next_min > heap[1]) {
                seq_t result = heap[1];
                free(heap);
                return result;
            }
        }
    }
}
