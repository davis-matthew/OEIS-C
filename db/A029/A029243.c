/*
 * A029243  Expansion of 1/((1-x^2)*(1-x^9)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 9, 11, 12
 *            Computation is done with the classic coin‑change dynamic
 *            program: dp[0] = 1; for each part p: for i = p .. n: dp[i] +=
 *            dp[i - p]; The table is rebuilt only when a larger index is
 *            requested
 * Range:     u8 n <= 137, u16 n <= 960, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;                /* dynamic programming table */
static idx_t  dp_cap = 0;               /* allocated capacity (number of slots) */
static idx_t  dp_len = 0;               /* number of indices that have been
                                            computed (next free index) */
static const idx_t parts[4] = {2, 9, 11, 12};

/* Rebuild the DP table up to (and including) index n.  This is done
   from scratch each time a larger index is needed. */
static void recompute_upto(idx_t n)
{
    /* Ensure the array is large enough.  Allocate exactly n+1 slots. */
    if (n + 1 > dp_cap) {
        seq_t *tmp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!tmp) abort();           /* fatal error on allocation failure */
        free(dp);
        dp = tmp;
        dp_cap = n + 1;
    }

    /* Clear all table entries that will be used.  The rest may contain
       old garbage but will never be read. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;

    dp[0] = (seq_t)1;                /* only the empty partition of 0 */

    /* Classic unbounded‑knapsack (coin change) DP: iterate over
       coin types first to avoid order‑counting. */
    for (int k = 0; k < 4; ++k) {
        idx_t p = parts[k];
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    dp_len = n + 1;                   /* indexes 0..n have been computed */
}

seq_t a(idx_t n)
{
    if (!dp) {
        /* First call: allocate a small table.  Size is irrelevant
           because recompute_upto will resize it promptly. */
        dp_cap = 16;
        dp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        if (!dp) abort();
        dp_len = 0;
    }

    if (dp_len <= n)
        recompute_upto(n);

    return dp[n];
}
