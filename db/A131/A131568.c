/*
 * A131568  a(n) = sum of numbers which in base 2 contain exactly n digits 1
 *          and not more than n digits 0.
 * https://oeis.org/A131568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (4^n - 1) * binomial(2*n, n) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t pow4 = pow_uint(4, n);           /* 4^n as seq_t */
    pow4 -= (seq_t)1;                     /* 4^n - 1 */
    seq_t bin = binomial(2 * n, n);       /* binomial(2*n, n) */
    pow4 *= bin;                          /* (4^n - 1) * binomial(2*n, n) */
    pow4 /= (seq_t)2;                     /* divide by 2 */
    return pow4;
}
