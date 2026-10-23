/*
 * A025434  Number of partitions of n into 10 nonzero squares.
 * https://oeis.org/A025434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 10 nonzero squares. a(n) =
 *            [x^n y^10] product_{k>=1} 1/(1 - y x^(k^2))
 * Range:     u8 n <= 239, u16 n <= 1454, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised table: dp[cnt][sum] stored as dp[cnt * stride + sum] */
    static seq_t *dp = NULL;
    static idx_t dp_n = 0;          /* largest n for which dp has been built */
    static size_t stride = 0;       /* stride = dp_n + 1 */

    /* Reallocate table if we need to handle a larger n or never allocated */
    if (dp == NULL || n > dp_n)
    {
        idx_t new_n = n;
        size_t new_stride = (size_t)new_n + 1;          /* number of sums */
        size_t new_size = (size_t)11 * new_stride;      /* 0..10 parts */

        seq_t *new_dp = NULL;
#ifdef __cplusplus
        new_dp = new seq_t[new_size];
#else
        new_dp = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (new_dp == NULL) return 0;  /* out of memory; unlikely for n <= 10000 */
#endif
        /* initialize to zero */
        for (size_t i = 0; i < new_size; ++i)
            new_dp[i] = 0;

        /* base case: 0 parts sum to 0 */
        new_dp[0] = 1;

        /* dynamic programming: iterate over square values */
        for (idx_t sq = 1; sq * sq <= new_n; ++sq)
        {
            idx_t val = sq * sq;           /* current square value */
            size_t val_u64 = (size_t)val;  /* for indexing */

            for (idx_t cnt = 1; cnt <= 10; ++cnt)
            {
                size_t cnt_off  = (size_t)cnt  * new_stride;
                size_t prev_off = (size_t)(cnt-1) * new_stride;

                for (idx_t sum = val; sum <= new_n; ++sum)
                {
                    size_t sum_u64 = (size_t)sum;
                    new_dp[cnt_off + sum_u64] +=
                        new_dp[prev_off + (sum_u64 - val_u64)];
                }
            }
        }

        /* free old buffer if any */
#ifdef __cplusplus
        delete[] dp;
#else
        free(dp);
#endif
        dp     = new_dp;
        dp_n   = new_n;
        stride = new_stride;
    }

    /* return the count for exactly 10 parts and the requested sum */
    return dp[10 * stride + (size_t)n];
}
