/*
 * A140657  Powers of 2 with 3 alternatingly added and subtracted.
 * https://oeis.org/A140657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n + 3*(-1)^n
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t power = pow2(n);   /* compute 2^n safely for any seq_t size */
    return (n & 1) ? (power - 3) : (power + 3);
}
