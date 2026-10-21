/*
 * A073377  Seventh convolution of A001045(n+1) (generalized (1,2)-Fibonacci),
 *          n>=0, with itself.
 * https://oeis.org/A073377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(n-k+7,7) * binomial(n-k,k) * 2^k
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 17, u64 n <= 41, u128 n <= 98,
 *            big any n
 * Helpers:   _my_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_my_binom.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t t1 = _my_binom(n - k + 7, 7);
        seq_t t2 = _my_binom(n - k, k);
        seq_t t3 = (seq_t)1 << k;           /* 2^k */
        seq_t term = t1 * t2 * t3;
        res += term;
    }
    return res;
}
