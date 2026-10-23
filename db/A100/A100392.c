/*
 * A100392  Numbers k such that A006530(k-1) < A006530(k) > A006530(k+1).
 * https://oeis.org/A100392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   lpf_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A100392
 * Definition: a(n) = the k such that the largest prime factor of k-1
 *              is less than that of k, and that of k is greater than
 *              that of k+1.  Offset = 1.
 * Computation method:
 *   Incrementally scan k = 3, 4, …, computing the largest prime
 *   factor of k-1, k, k+1 by simple trial division.  Whenever the
 *   local‑maximum condition holds, store k in a static cache.
 *   The function returns the (n‑1)th element of this cache.
 *   The algorithm runs in O(k) time per term, where k is the index
 *   of the next term found (≈1.5 · n for the first 10 000 terms).
 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/lpf_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; 0 is undefined */

    /* Static cache of sequence values */
    static seq_t *seq_arr = NULL;
    static size_t seq_cap = 0;
    static idx_t seq_size = 0;

    /* Current value of k to test, starting with the first
       admissible candidate 3 (k=2 never satisfies the condition). */
    static idx_t next_k = 3;

    while (seq_size < n) {
        seq_t lp_prev = lpf_uint64((uint64_t)(next_k - 1));
        seq_t lp_cur  = lpf_uint64((uint64_t)next_k);
        seq_t lp_next = lpf_uint64((uint64_t)(next_k + 1));

        if (lp_prev < lp_cur && lp_cur > lp_next) {
            /* Append to cache */
            if (seq_size == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (!tmp) { free(seq_arr); exit(1); }
                seq_arr = tmp;
            }
            seq_arr[seq_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return seq_arr[n - 1];
}
