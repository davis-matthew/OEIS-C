/*
 * A161228  Number of partitions of n into Catalan numbers A000108 where every
 *          part appears at least 3 times.
 * https://oeis.org/A161228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into Catalan numbers where each
 *            part appears at least 3 times. Computed with a
 *            generating‑function DP that multiplies ( 1 + x^{3C} + x^{4C} + …
 *            ) over all Catalan numbers C ≤ n
 * Range:     u8 n <= 76, u16 n <= 365, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Catalan numbers ≤ 10000 (unique values) */
    static const idx_t catalan_vals[] = {1, 2, 5, 14, 42, 132, 429, 1430, 4862};
    static const int catalan_count =
        sizeof(catalan_vals)/sizeof(catalan_vals[0]);

    /* Precomputed DP for n up to MAXN */
    #define MAXN 10000
    static seq_t dp[MAXN + 1];
    static seq_t newdp[MAXN + 1];
    static int computed = 0;      /* 0 → not yet, 1 → done   */

    if (!computed) {
        /* Initialize dp */
        for (idx_t i = 0; i <= MAXN; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* Process each Catalan value */
        for (int ci = 0; ci < catalan_count; ++ci) {
            idx_t v = catalan_vals[ci];
            /* Clear newdp */
            for (idx_t i = 0; i <= MAXN; ++i)
                newdp[i] = 0;

            /* Convolve */
            for (idx_t sum = 0; sum <= MAXN; ++sum) {
                seq_t ways = dp[sum];
                if (!ways) continue;

                /* Skip this part */
                newdp[sum] += ways;

                /* Use it at least 3 times */
                for (idx_t s = sum + 3 * v; s <= MAXN; s += v)
                    newdp[s] += ways;
            }

            /* Copy back to dp */
            for (idx_t i = 0; i <= MAXN; ++i)
                dp[i] = newdp[i];
        }
        computed = 1;
    }

    if (n <= MAXN)
        return dp[n];

    /* For n > MAXN, compute on demand (rarely used). */
    seq_t *dp_big = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *newdp_big = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp_big || !newdp_big) {
        free(dp_big);
        free(newdp_big);
        return 0;   /* out of memory */
    }

    for (idx_t i = 0; i <= n; ++i)
        dp_big[i] = 0;
    dp_big[0] = 1;

    for (int ci = 0; ci < catalan_count; ++ci) {
        idx_t v = catalan_vals[ci];
        if (v > n) continue;
        for (idx_t i = 0; i <= n; ++i)
            newdp_big[i] = 0;

        for (idx_t sum = 0; sum <= n; ++sum) {
            seq_t ways = dp_big[sum];
            if (!ways) continue;
            newdp_big[sum] += ways;
            for (idx_t s = sum + 3 * v; s <= n; s += v)
                newdp_big[s] += ways;
        }

        for (idx_t i = 0; i <= n; ++i)
            dp_big[i] = newdp_big[i];
    }

    seq_t result = dp_big[n];
    free(dp_big);
    free(newdp_big);
    return result;
}
