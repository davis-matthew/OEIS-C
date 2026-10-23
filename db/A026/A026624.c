/*
 * A026624  a(n) = Sum_{j=0..n} Sum_{k=0..j} A026615(j, k).
 * https://oeis.org/A026624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 7*2^(n-1) - 2*(n+1) for n >= 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = pow2(n - 1);              /* 2^(n-1)      */
    res *= (seq_t)7;                      /* 7*2^(n-1)    */
    res -= (seq_t)2 * (seq_t)(n + 1);     /* subtract 2*(n+1) */
    return res;
}
