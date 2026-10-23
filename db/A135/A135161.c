/*
 * A135161  a(n) = 7^n - 5^n - 3^n - 2^n. Constants are the prime numbers in
 *          decreasing order.
 * https://oeis.org/A135161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 7^n - 5^n - 3^n - 2^n
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t p7 = pow_uint(7, n);
    seq_t p5 = pow_uint(5, n);
    seq_t p3 = pow_uint(3, n);
    seq_t p2 = pow_uint(2, n);
    return p7 - p5 - p3 - p2;
}
