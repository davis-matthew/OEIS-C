/*
 * A160948  Numbers n that are multiples of the digital sum of n-1.
 * https://oeis.org/A160948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth integer k such that k is a multiple of the digital
   sum of k-1. The function searches from k = 2 upwards and collects the
   nth such k. */
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; value for 0 is irrelevant */
        return (seq_t)0;

    idx_t    found = 0;
    seq_t    cand  = (seq_t)2;   /* start searching from 2 */

    while (found < n) {
        seq_t ds = digit_sum(cand - (seq_t)1);  /* ds(k-1) */

        if (cand % ds == (seq_t)0) {
            ++found;
            if (found == n)
                return cand;              /* nth term found */
        }
        ++cand;                           /* next candidate */
    }

    /* unreachable, but required for proper return type */
    return (seq_t)0;
}
