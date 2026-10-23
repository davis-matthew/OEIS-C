/*
 * A029264  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A029264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Number of partitions of n into parts 3, 4, 8, and 11
 * Range:     u8 n <= 105, u16 n <= 733, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed with a classic coin‑change dynamic program. */

/* NOTE: The implementation assumes seq_t and idx_t are defined elsewhere. */
#include <stdlib.h>

/* static cache for the DP table */
static seq_t *dp_cache = NULL;      /* points to dp[0..max_n] */
static idx_t   dp_max   = 0;        /* maximum i for which dp_cache[i] is defined */

/* the allowed parts */
static const int parts[] = {3, 4, 8, 11};
static const size_t part_cnt = 4;

seq_t a(idx_t n)
{
    /* if we haven't computed anything yet, or need to extend the table, */
    /* recompute it from scratch up to the new maximum n */
    if (dp_cache == NULL || n > dp_max)
    {
        /* allocate or extend the cache */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp_cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
        {
            /* allocation failure: return 0 as a safe fallback */
            return (seq_t)0;
        }
        dp_cache = tmp;
        dp_max   = n;

        /* zero the new table */
        for (size_t i = 0; i <= n; ++i)
        {
            dp_cache[i] = (seq_t)0;
        }
        /* base case */
        dp_cache[0] = (seq_t)1;

        /* classic coin‑change DP: outer loop over parts, inner over sums */
        for (size_t pi = 0; pi < part_cnt; ++pi)
        {
            idx_t p = (idx_t)parts[pi];
            if (p > n)
                continue;
            for (idx_t i = p; i <= n; ++i)
            {
                dp_cache[i] += dp_cache[i - p];
            }
        }
    }

    /* now dp_cache[n] is valid */
    return dp_cache[n];
}
