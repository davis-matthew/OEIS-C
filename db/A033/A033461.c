/*
 * A033461  Number of partitions of n into distinct squares.
 * https://oeis.org/A033461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of partitions of n into distinct squares. This
 *            uses a classic 0‑1 knapsack over the square numbers k^2
 * Range:     u8 n <= 654, u16 n <= 2340, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Integer square root: floor(sqrt(n)) for idx_t (=uint64_t). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while (r + 1 <= n / (r + 1))
        ++r;
    return r;
}

/* Compute a(n).  DP is cached so that repeated calls are fast. */
seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t dp_max = 0;

    /* Build or extend the DP table up to the requested n. */
    if (dp == NULL || n > dp_max) {
        /* Allocate (or enlarge) the table.  Need n+1 entries (0..n). */
        seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (new_dp == NULL)            /* allocation failure – give up */
            return (seq_t)0;
        dp = new_dp;

        /* Zero all entries; then set dp[0] = 1. */
        memset(dp, 0, (n + 1) * sizeof(seq_t));
        dp[0] = (seq_t)1;

        /* 0‑1 knapsack over all square numbers <= n. */
        idx_t limit = isqrt_u64(n);
        for (idx_t k = 1; k <= limit; ++k) {
            idx_t sq = k * k;
            for (idx_t i = n; i >= sq; --i)
                dp[i] += dp[i - sq];
        }
        dp_max = n;
    }
    return dp[n];
}
