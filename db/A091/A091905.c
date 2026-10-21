/*
 * A091905  Expansion of (1-4*x)/((1+4*x)*(1-8*x)).
 * https://oeis.org/A091905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^n/3 + 2*(-4)^n/3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pow8 = pow_uint(8, n);
    seq_t pow4 = pow_uint(4, n);
    seq_t two_pow4 = pow4 * (seq_t)2;
    seq_t sum;
    if ((n & 1) == 0)      /* n even: add 2*4^n */
        sum = pow8 + two_pow4;
    else                   /* n odd: subtract 2*4^n */
        sum = pow8 - two_pow4;
    return sum / (seq_t)3;
}
