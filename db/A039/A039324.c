/*
 * A039324  Numbers whose base-7 representation has the same nonzero number of
 *          3's and 5's.
 * https://oeis.org/A039324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose base-7 representation has the same
 *            nonzero number of 3's and 5's
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined for n < 1, return 0 */
    idx_t target = n;
    seq_t x = 1;

    while (1) {
        seq_t tmp = x;
        int c3 = 0, c5 = 0;

        /* Count digits 3 and 5 in base-7 representation of x */
        while (tmp > (seq_t)0) {
            int d = (int)(tmp % (seq_t)7);
            if (d == 3) c3++;
            else if (d == 5) c5++;
            tmp /= (seq_t)7;
        }

        if (c3 != 0 && c3 == c5) {
            if (--target == 0)
                return x;
        }

        ++x;
    }
}
