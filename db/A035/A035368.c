/*
 * A035368  Number of partitions of n into parts 5k or 5k+2.
 * https://oeis.org/A035368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts partitions of n into parts 5k (k ≥ 1) or 5k+2 (k ≥
 *            0)
 * Range:     u8 n <= 47, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses ordinary coin‑change DP: for each allowed part
   weight w, dp[i] += dp[i‑w] for i ≥ w.  The table is recomputed from
   scratch whenever a larger n is requested. */

#include <stdlib.h>

static seq_t *dp     = NULL;  /* cache of partition numbers */
static idx_t max_n   = 0;     /* largest n for which dp was computed */

/* Return the number of partitions of n into parts 5k or 5k+2. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; we simply ignore n = 0. */
    if (n == 0)
        return (seq_t)0;

    /* If we haven't computed up to n yet, recompute the full table. */
    if (n > max_n) {
        /* Allocate a new table of size n+1. */
        seq_t *new_dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!new_dp)
            return (seq_t)0;            /* allocation failed: safe fallback */

        /* Initialise the table. */
        for (idx_t i = 0; i <= n; ++i)
            new_dp[i] = (seq_t)0;
        new_dp[0] = (seq_t)1;          /* empty partition */

        /* 1. Allowed parts of the form 5k+2, k ≥ 0 (start at 2, step 5). */
        for (idx_t part = 2; part <= n; part += 5) {
            for (idx_t i = part; i <= n; ++i)
                new_dp[i] += new_dp[i - part];
        }

        /* 2. Allowed parts of the form 5k, k ≥ 1 (start at 5, step 5). */
        for (idx_t part = 5; part <= n; part += 5) {
            for (idx_t i = part; i <= n; ++i)
                new_dp[i] += new_dp[i - part];
        }

        /* Free the old cache and store the new one. */
        if (dp)
            free(dp);
        dp     = new_dp;
        max_n  = n;
    }

    /* Return the precomputed answer. */
    return dp[n];
}
