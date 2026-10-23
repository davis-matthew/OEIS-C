/*
 * A195256  O.g.f.: Sum_{n>=0} 4*(n+4)^(n-1)*x^n/(1+n*x)^n.
 * https://oeis.org/A195256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n) = (n-1)*a(n-1) + 4^n for n >= 1 (same for n=1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;      /* a(0) */
    seq_t pow4   = 1;      /* 4^0 */
    for (idx_t i = 1; i <= n; ++i)
    {
        pow4 = pow4 * 4;                       /* 4^i */
        result = result * (seq_t)(i - 1) + pow4;
    }
    return result;
}
