/*
 * A025868  Expansion of 1/((1-x^4)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A025868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 7, and 9.
 *            Computation is performed with a standard "coin change" DP which
 *            counts combinations (order does not matter). The DP is
 *            recomputed from scratch whenever a larger index is requested
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const idx_t parts[3] = {4, 7, 9};

static seq_t *dp = NULL;          /* cache of computed values   */
static idx_t max_computed = 0;    /* largest index that has been filled  */

/* recompute dp[0 .. up] from scratch */
static void recompute(idx_t up)
{
    /* initialize all entries to zero */
    for (idx_t i = 0; i <= up; ++i)
        dp[i] = 0;

    dp[0] = 1;  /* empty partition */

    /* iterate over the part sizes; for each size add its
       contribution to higher indices.  This is the standard
       coin‐change DP that counts unordered combinations. */
    for (int pi = 0; pi < 3; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= up; ++i)
            dp[i] += dp[i - p];
    }
}

seq_t a(idx_t n)
{
    /* First call – allocate space for n+1 entries. */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (dp == NULL)
            return (seq_t)0;   /* out‑of‑memory fallback */
        max_computed = n;
        recompute(n);
        return dp[n];
    }

    /* Expand the cache if n is beyond what we have already computed. */
    if (n > max_computed) {
        seq_t *new_dp = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
        if (new_dp == NULL)
            return (seq_t)0;   /* out‑of‑memory fallback */
        dp = new_dp;
        /* recompute all values up to n */
        recompute(n);
        max_computed = n;
    }

    return dp[n];
}
