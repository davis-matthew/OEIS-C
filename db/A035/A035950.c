/*
 * A035950  Number of partitions of n into parts not of the form 13k, 13k+2 or
 *          13k-2. Also number of partitions with 1 part of size 1 and
 *          differences between parts at distance 5 are greater than 1.
 * https://oeis.org/A035950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not of the form 13k,
 *            13k+2, or 13k-2. Implementation uses a standard unbounded
 *            knapsack DP recomputed for each new maximum n
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state for memoization of computed values. */
    static seq_t *dp       = NULL; /* dp[s] = number of partitions of s */
    static idx_t dp_max_n  = 0;    /* largest n that has been computed */

    if (!dp || n > dp_max_n) {                /* need to compute up to n */
        /* Resize storage */
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failure */
        dp = tmp;

        /* Initialise array for fresh computation. */
        for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
        dp[0] = 1;                            /* empty partition */

        /* Unbounded knapsack DP over all allowed part sizes. */
        for (idx_t part = 1; part <= n; ++part) {
            uint64_t r = part % 13ULL;
            if (r == 0 || r == 2 || r == 11)  /* forbidden residues */
                continue;

            for (idx_t sum = part; sum <= n; ++sum)
                dp[sum] += dp[sum - part];
        }

        dp_max_n = n;                          /* update cache state */
    }

    return dp[n];    /* return stored result for requested n */
}
