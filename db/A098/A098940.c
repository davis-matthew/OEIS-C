/*
 * A098940  a(n) = (2^n - 1)^7 - 2.
 * https://oeis.org/A098940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2^n - 1)^7 - 2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 2^n as a seq_t value */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;          /* multiplication only */

    /* base = 2^n - 1 */
    seq_t base = pow2 - 1;

    /* compute base^7 by repeated multiplication */
    seq_t res = base;
    for (int i = 1; i < 7; ++i)
        res *= base;

    /* subtract 2 */
    res -= 2;

    return res;
}
