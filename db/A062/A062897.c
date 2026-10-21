/*
 * A062897  Number and its reversal are both multiples of 2.
 * https://oeis.org/A062897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose first and last decimal digits
 *            are even
 * Range:     u8 n <= 52, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num   = 1;

    while (true) {
        ++num;                           /* test next integer */
        if ((num & 1) != 0) continue;    /* must be even */

        /* find the most significant digit */
        idx_t msd = num;
        while (msd >= 10) msd /= 10;
        if ((msd & 1) != 0) continue;    /* first digit must be even */

        ++count;
        if (count == n)
            return (seq_t)num;           /* cast to seq_t width */
    }
}
