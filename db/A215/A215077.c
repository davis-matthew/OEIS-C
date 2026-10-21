/*
 * A215077  Binomial convolution of sum of consecutive powers.
 * https://oeis.org/A215077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*Sum_{j=1..k} j^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (same as A215077) */

#include "../utils/big_pow.h"   /* big_pow() */

seq_t a(idx_t n)
{
    seq_t ans   = 0;
    seq_t binom = 1;           /* C(n,0) */
    seq_t S     = 0;           /* Sum_{j=1..k} j^n for current k */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term for current k */
        ans += binom * S;

        if (k == n) break;    /* last iteration */

        /* Update S to incorporate (k+1)^n */
        S += big_pow((seq_t)(k + 1), n);

        /* Update binomial coefficient to C(n,k+1) */
        binom = binom * (n - k) / (k + 1);
    }

    return ans;
}
