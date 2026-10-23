/*
 * A022056  Theta series of D_25 lattice.
 * https://oeis.org/A022056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer vectors v ∈ ℤ^25 with even coordinate
 *            sum and ∑ v_i^2 = 2n. Computed by dynamic programming over the
 *            25 coordinates. The function returns a(n) for any n ≥ 0
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 34, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already-computed values */
    static idx_t cache_n = 0;
    static seq_t *cache   = NULL;

    /* Return cached value if available */
    if (cache && n <= cache_n)
        return cache[n];

    /* Compute up to the requested n */
    idx_t new_n   = n;
    idx_t max_sum = new_n * 2;              /* we need sums up to 2n */
    size_t len    = (size_t)(max_sum + 1) * 2;  /* two parity states per sum */

    /* Allocate DP arrays */
    seq_t *dp  = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *tmp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp || !tmp)
    {
        free(dp);
        free(tmp);
        return (seq_t)0;                     /* unlikely in contest */
    }

    /* Initial state: sum = 0, even parity */
    dp[0] = (seq_t)1;                      /* index 0 corresponds to sum=0, parity=0 */

    /* Process each of the 25 coordinates */
    for (int dim = 0; dim < 25; ++dim)
    {
        /* clear next layer */
        for (size_t i = 0; i < len; ++i)
            tmp[i] = (seq_t)0;

        /* iterate over current sums */
        for (idx_t sum = 0; sum <= max_sum; ++sum)
        {
            seq_t val_even = dp[sum * 2 + 0];
            seq_t val_odd  = dp[sum * 2 + 1];

            if (!val_even && !val_odd)
                continue;                   /* nothing to propagate */

            /* try all possible absolute values k for the new coordinate */
            for (idx_t k = 0; k * k <= max_sum - sum; ++k)
            {
                idx_t e          = k * k;
                idx_t new_sum    = sum + e;
                seq_t mult       = (k == 0) ? (seq_t)1 : (seq_t)2;
                idx_t kpar       = k & 1;   /* parity of k */

                /* update from even parity states */
                if (val_even)
                {
                    seq_t add = val_even * mult;
                    tmp[new_sum * 2 + kpar] += add;
                }

                /* update from odd parity states */
                if (val_odd)
                {
                    seq_t add = val_odd * mult;
                    tmp[new_sum * 2 + (kpar ^ 1)] += add;
                }
            }
        }

        /* swap dp and tmp for the next dimension */
        seq_t *swap = dp;
        dp = tmp;
        tmp = swap;
    }

    /* Allocate new cache with all values up to new_n */
    seq_t *new_cache = (seq_t *)calloc((size_t)(new_n + 1), sizeof(seq_t));
    if (!new_cache)
    {
        free(dp);
        free(tmp);
        return (seq_t)0;                     /* allocation failure */
    }

    /* Store even-parity counts for even sums (2n) */
    for (idx_t s = 0; s <= max_sum; s += 2)
    {
        new_cache[s / 2] = dp[s * 2 + 0];
    }

    /* Free DP arrays and previous cache */
    free(dp);
    free(tmp);
    if (cache)
        free(cache);

    /* Update cache */
    cache    = new_cache;
    cache_n  = new_n;

    return cache[n];
}
