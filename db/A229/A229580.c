/*
 * A229580  Number of defective 3-colorings of an n X 2 0..2 array connected
 *          horizontally and antidiagonally with exactly one mistake, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n - 1) * 2^(2n - 3) for n > 1; a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* outside the official range, undefined */
    if (n == 1)
        return (seq_t)1;

    /* exponent = 2*n - 3  (valid for n >= 2) */
    idx_t exp = n * 2 - 3;
    seq_t pow = pow2(exp);           /* 2^(2n-3) */
    seq_t factor = (seq_t)n * 2 - 1; /* 2n-1 */
    return pow * factor;
}
