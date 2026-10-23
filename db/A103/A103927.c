/*
 * A103927  Number of partitions of n into parts but with two kinds of parts
 *          of sizes 1 to 8.
 * https://oeis.org/A103927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: (∏_{k=1..8} 1/(1-x^k))^2⋅∏_{j≥9} 1/(1-x^j)
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Number of partitions of n into parts but with two kinds of parts of sizes 1 to 8. */
seq_t a(idx_t n)
{
    /* Static cache for repeated calls */
    static idx_t cache_n = (idx_t)-1;
    static seq_t *dp    = NULL;
    static size_t dp_cap = 0;

    /* Recompute when we need a larger array */
    if (dp == NULL || n >= dp_cap)
    {
        /* Allocate new array (zero‑initialized) */
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_dp = (seq_t*)calloc(new_cap, sizeof(seq_t));
        if (!new_dp) return 0;               /* allocation failure */

        if (dp != NULL) free(dp);            /* discard old array */
        dp      = new_dp;
        dp_cap  = new_cap;

        /* Base value */
        dp[0] = 1;

        /* Parts 1..8, two colours each (product (1-x^k)^(-2)) */
        for (idx_t k = 1; k <= 8 && k <= n; ++k)
        {
            for (int col = 0; col < 2; ++col)        /* two colours */
            {
                for (idx_t i = k; i <= n; ++i)
                    dp[i] += dp[i - k];
            }
        }

        /* Parts 9..n, single colour (product (1-x^j)^(-1)) */
        for (idx_t k = 9; k <= n; ++k)
        {
            for (idx_t i = k; i <= n; ++i)
                dp[i] += dp[i - k];
        }
    }

    return dp[n];
}
