/*
 * A039229  Numbers whose base-12 representation has the same number of 1's
 *          and 11's.
 * https://oeis.org/A039229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A039229: numbers whose base‑12 representation has the same
 * number of 1's and 11's.
 * Algorithm: linear search counting the digits 1 and 11 in base‑12. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t   count = 0;          /* how many qualifying numbers found */
    seq_t   val   = (seq_t)0;   /* current integer to test */

    for (;; ++val) {
        /* count 1's and 11's in the base‑12 representation of val */
        unsigned c1   = 0;
        unsigned c11  = 0;
        seq_t   tmp   = val;

        while (tmp) {
            seq_t rem = tmp % (seq_t)12;
            if (rem == (seq_t)1)       ++c1;
            else if (rem == (seq_t)11) ++c11;
            tmp /= (seq_t)12;
        }

        /* zero also satisfies the condition (c1 == c11 == 0) */
        if (c1 == c11) {
            ++count;
            if (count == n)
                return val;        /* n‑th term found */
        }
    }
}
