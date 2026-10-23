/*
 * A029387  Expansion of g.f. 1/((1 - x^5)*(1 - x^6)*(1 - x^9)*(1 - x^10)).
 * https://oeis.org/A029387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 5, 6, 9 and 10. */
/* The algorithm recomputes the DP table for the required n on each call
 * using the standard unbounded‑knapsack dynamic programming
 * (each part considered once, from smallest to largest).
 * Complexity: O(n) per call (parts count is constant). */

#include <stdlib.h>

static seq_t *dp = NULL;          /* dynamic programming table */
static idx_t  dp_cap = 0;         /* allocated size of dp */

seq_t a(idx_t n)
{
    /* Allocate or grow the DP buffer if necessary */
    if (dp == NULL)
    {
        dp_cap = n + 1;
        dp = (seq_t *)malloc(sizeof(seq_t) * dp_cap);
    }
    else if (dp_cap <= n)
    {
        dp_cap = n + 1;
        dp = (seq_t *)realloc(dp, sizeof(seq_t) * dp_cap);
    }

    /* Re-initialize dp[0..n] */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* The set of allowed parts */
    static const idx_t parts[4] = {5, 6, 9, 10};

    /* Unbounded‑knapsack DP */
    for (size_t pi = 0; pi < 4; ++pi)
    {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    return dp[n];
}
