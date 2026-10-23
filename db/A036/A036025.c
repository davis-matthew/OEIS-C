/*
 * A036025  Number of partitions of n into parts not of form 4k+2, 20k, 20k+3
 *          or 20k-3. Also number of partitions in which no odd part is
 *          repeated, with 1 part of size less than or equal to 2 and where
 *          differences between parts at distance 4 are greater than 1 when
 *          the smallest part is odd and greater than 2...
 * https://oeis.org/A036025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP for partitions of n into parts not of form 4k+2, 20k,
   20k+3 or 20k-3.  The DP array is recomputed from scratch each time a
   larger n is requested.  All arithmetic is performed in seq_t. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: true if p is forbidden */
static inline bool forbidden_part(idx_t p)
{
    /* 4k+2 */
    if ((p & 3) == 2) return true;
    /* 20k, 20k+3, 20k-3 */
    idx_t r = p % 20;
    return (r == 0 || r == 3 || r == 17);
}

/* Static cache */
static seq_t *dp = NULL;      /* dp[i] = a(i) for 0 <= i <= dp_max_n */
static idx_t dp_size = 0;     /* capacity of dp array */
static idx_t dp_max_n = 0;    /* largest n for which dp[n] is correct */

/* Recompute dp for all 0 <= i <= n from scratch */
static void recompute_dp(idx_t n)
{
    /* allocate or grow */
    if (dp == NULL)
    {
        dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    }
    else
    {
        /* round up to handle potential resizing */
        dp = (seq_t *)realloc(dp, ((size_t)n + 1) * sizeof(seq_t));
        /* zero newly allocated part (if any) */
        for (idx_t i = dp_size; i <= n; ++i)
            dp[i] = 0;
    }
    dp_size = (size_t)n + 1;

    /* initialize DP */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1; /* one way to partition zero */

    /* standard partition DP using only allowed parts */
    for (idx_t p = 1; p <= n; ++p)
    {
        if (forbidden_part(p)) continue;
        for (idx_t s = p; s <= n; ++s)
        {
            dp[s] += dp[s - p];
        }
    }
    dp_max_n = n;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out‑of‑range; behavior irrelevant */
    if (n > dp_max_n) recompute_dp(n);
    return dp[n];
}
