/*
 * A084090  Numbers k such that k is divisible by 3 or the exponent of the
 *          highest power of 2 dividing k is odd. Complement of A084087.
 * https://oeis.org/A084090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084090: a(n) = the nth non‑negative integer k such that
   k is divisible by 3 or the exponent of the highest power of 2
   dividing k is odd.  The sequence starts 0,2,3,6,8,9,...
   a(1) = 0.  The function works for any n ≥ 1; it scans k
   from 0 upward until the nth qualifying number is found. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;      /* how many qualifying numbers have been seen */
    seq_t k = 0;          /* current candidate number */

    for (;; k++) {
        /* If k is divisible by 3, it is in the sequence. */
        if ((k % (seq_t)3) == 0) {
            count++;
        } else {
            /* k is not divisible by 3; check whether the
               exponent of 2 (v2(k)) is odd. */
            seq_t t = k;
            idx_t tz = 0;            /* number of trailing zeros in k */
            while ((t & 1) == 0) {
                t >>= 1;
                tz++;
            }
            if ((tz & 1) == 1)      /* odd exponent of 2 */
                count++;
        }

        /* When we have reached the nth qualifying number, return it. */
        if (count == n)
            return k;
    }
}
