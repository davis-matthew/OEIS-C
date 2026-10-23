/*
 * A182268  Denominator of Euler(n, 3/14).
 * https://oeis.org/A182268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n * 2^n if n is even, * a(n) = 7^n * 2^(n-2) if n is
 *            odd (n>=3), * a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t seven_pow = pow_uint(7, n);
    seq_t two_pow  = (n & 1) ? pow_uint(2, n - 2) : pow_uint(2, n);

    return seven_pow * two_pow;
}
