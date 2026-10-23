/*
 * A024135  a(n) = 11^n - n^8.
 * https://oeis.org/A024135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 11^n - n^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 18, i128 n <= 36,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    return pow_uint(11, n) - pow_uint(n, 8);
}
