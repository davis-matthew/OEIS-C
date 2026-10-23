/*
 * A039164  Numbers whose base-11 representation has the same number of 0's
 *          and 9's.
 * https://oeis.org/A039164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer whose base‑11 representation has an
 * equal number of 0’s and 9’s.  The search is performed by examining the
 * numbers in ascending order. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t candidate = (seq_t)1;

    idx_t found = 0;

    while (1) {
        int count0 = 0;
        int count9 = 0;

        seq_t tmp = candidate;
        while (tmp > 0) {
            seq_t digit = tmp % 11;
            if (digit == 0) count0++;
            else if (digit == 9) count9++;
            tmp /= 11;
        }

        if (count0 == count9) {
            found++;
            if (found == n)
                return candidate;
        }
        candidate += 1;
    }
}
