/*
 * A166978  a(n) = 4*( 1-(-1)^n) -2^n.
 * https://oeis.org/A166978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*(1-(-1)^n) - 2^n
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 2^n as a seq_t. */
    seq_t pow2 = pow_uint(2, n);

    /* (-1)^n is +1 for even n, -1 for odd n. */
    if ((n & 1) == 0) {
        /* Even n: a(n) = -2^n. */
        return -pow2;
    } else {
        /* Odd n: a(n) = 8 - 2^n. */
        return (seq_t)8 - pow2;
    }
}
