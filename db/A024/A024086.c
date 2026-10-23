/*
 * A024086  a(n) = 7^n - n^11.
 * https://oeis.org/A024086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute 7^n */
    seq_t p7 = pow_uint(7, n);
    /* compute n^11 */
    seq_t p11 = pow_uint(n, 11);
    /* return difference */
    return p7 - p11;
}
