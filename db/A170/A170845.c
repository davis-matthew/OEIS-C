/*
 * A170845  G.f.: Product_{k>=1} (1 + 3x^(2^k-1) + x^(2^k)).
 * https://oeis.org/A170845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in the infinite product Π_{k≥1}
 *            (1 + 3 x^(2^k-1) + x^(2^k)). The following implementation
 *            computes a(n) by dynamic programming over the binary‐exponent
 *            factors
 * Range:     u8 n <= 56, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a DP array of size n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t*)malloc(sz * sizeof(seq_t));
    if (dp == NULL) return 0;                     /* allocation failed */

    /* initialise dp[0] = 1, others = 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* iterate over factors (k = 1,2,...) */
    for (idx_t k = 1;; ++k)
    {
        seq_t power2 = (seq_t)1 << k;      /* 2^k   */
        seq_t a_exp = power2 - (seq_t)1;   /* 2^k-1 */
        seq_t b_exp = power2;              /* 2^k   */

        /* stop when both exponents exceed n */
        if ((seq_t)a_exp > (seq_t)n && (seq_t)b_exp > (seq_t)n) break;

        /* in-place convolution: update dp from high to low indices */
        for (idx_t i = n; ; --i)
        {
            seq_t val = dp[i];
            if (val != 0)
            {
                seq_t tA = (seq_t)i + a_exp;
                if (tA <= (seq_t)n)
                    dp[(idx_t)tA] += val * (seq_t)3;

                seq_t tB = (seq_t)i + b_exp;
                if (tB <= (seq_t)n)
                    dp[(idx_t)tB] += val;
            }
            if (i == 0) break;   /* unsigned loop guard */
        }
    }

    seq_t result = dp[(idx_t)n];
    free(dp);
    return result;
}
