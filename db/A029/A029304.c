/*
 * A029304  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 6, 10, 12
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* The allowed parts */
    const idx_t parts[4] = {3, 6, 10, 12};

    /* Allocate an array of length n+1.  Use size_t for the allocation size
       to avoid overflow on the size value.  All indices are handled as
       idx_t, which is unsigned 64‑bit. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) {
        /* In an unlikely allocation failure, return 0.  The harness
           does not expect the function to abort. */
        return (seq_t)0;
    }

    /* Base case: one way to form 0, the empty sum. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Unbounded knapsack – accumulate contributions of each part. */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i)
            dp[i] = dp[i] + dp[i - p];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
