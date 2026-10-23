/*
 * A160985  Number of partitions of n where every part appears at least 15
 *          times.
 * https://oeis.org/A160985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #partitions of n in which every part size appears at
 *            least 15 times
 * Range:     u8 n <= 123, u16 n <= 273, u32 n <= 716, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* work only for n >= 1 */
    if (n < 15) return 0;  /* all values below 15 are zero */

    /* allocate arrays of length n+1 */
    size_t len = (size_t)(n + 1);
    seq_t *dp   = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *F    = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *sum15 = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *new_dp = (seq_t *)calloc(len, sizeof(seq_t));

    if (!dp || !F || !sum15 || !new_dp) {
        /* allocation failed – return 0 as safe default */
        free(dp); free(F); free(sum15); free(new_dp);
        return 0;
    }

    dp[0] = 1;  /* empty partition */

    /* we need only part sizes k with 15*k <= n */
    idx_t max_k = n / 15;
    for (idx_t k = 1; k <= max_k; ++k)
    {
        /* --- compute F[s] = sum_{t>=0, t*k <= s} dp[s - t*k] --- */
        idx_t limit = n;
        for (idx_t s = 0; s <= limit; ++s)
        {
            if (s >= k)
                F[s] = dp[s] + F[s - k];
            else
                F[s] = dp[s];
        }

        /* --- compute sum15[s] = sum_{t=0}^{14, t*k <= s} dp[s - t*k] --- */
        for (idx_t r = 0; r < k; ++r)
        {
            seq_t window = 0;
            idx_t count = 0;
            for (idx_t s = r; s <= limit; s += k)
            {
                /* add the newest term */
                window += dp[s];
                if (count >= 15)
                    window -= dp[s - 15 * k];
                ++count;
                sum15[s] = window;
            }
        }

        /* --- compute new_dp[s] = dp[s] + (F[s] - sum15[s]) --- */
        for (idx_t s = 0; s <= limit; ++s)
        {
            seq_t add = F[s] - sum15[s];
            new_dp[s] = dp[s] + add;
        }

        /* swap dp and new_dp for next iteration */
        seq_t *tmp = dp;
        dp = new_dp;
        new_dp = tmp;
    }

    /* result is in dp[n] */
    seq_t result = dp[n];

    /* cleanup */
    free(dp);
    free(F);
    free(sum15);
    free(new_dp);

    return result;
}
