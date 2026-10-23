/*
 * A039317  Numbers whose base-7 representation has the same nonzero number of
 *          1's and 5's.
 * https://oeis.org/A039317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers whose base-7 representation has the same nonzero number
 *            of 1's and 5's (OEIS A039317). Computation is performed by
 *            brute‑force search of natural numbers, counting the occurrences
 *            of digits 1 and 5 in base 7
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t value = 0;

    for (;; ++value) {
        seq_t x = value;
        int c1 = 0, c5 = 0;

        /* Count digits 1 and 5 in base 7 */
        while (x > 0) {
            int d = (int)(x % 7);
            if (d == 1) ++c1;
            else if (d == 5) ++c5;
            x /= 7;
        }

        if (c1 > 0 && c1 == c5) {
            ++count;
            if (count == n)
                return value;
        }
    }
}
