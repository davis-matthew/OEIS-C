/*
 * A029248  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^5)*(1-x^10)).
 * https://oeis.org/A029248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 606, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: number of partitions of n into parts 3, 4, 5 and 10.
   Computed by dynamic programming and memoized for successive calls. */

#include <stdlib.h>

static seq_t *dp = NULL;          /* array of computed values */
static idx_t  max_n = 0;          /* largest n that has been computed */
static const idx_t parts[4] = {3,4,5,10};

seq_t a(idx_t n)
{
    /* Recompute the table when a larger index is requested. */
    if (!dp || n > max_n) {
        size_t new_len = (size_t)n + 1;

        seq_t *new_dp = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_dp)            /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;

        for (size_t i = 0; i < new_len; ++i)
            new_dp[i] = 0;
        new_dp[0] = 1;          /* empty partition */

        /* Standard coin‑change DP: for each part, add its contributions. */
        for (int pi = 0; pi < 4; ++pi) {
            idx_t part = parts[pi];
            for (idx_t i = part; i <= n; ++i)
                new_dp[i] += new_dp[i - part];
        }

        free(dp);               /* discard old table */
        dp     = new_dp;
        max_n  = n;              /* new maximum index */
    }

    return dp[n];
}
