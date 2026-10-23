/*
 * A035995  Number of partitions of n into parts not of the form 23k, 23k+7 or
 *          23k-7. Also number of partitions with at most 6 parts of size 1
 *          and differences between parts at distance 10 are greater than 1.
 * https://oeis.org/A035995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            7, or 16 modulo 23. Computed by the classical partition DP
 * Range:     u8 n <= 17, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* Ensure we have enough space for indices 0 … n. */
    if (dp_size < n + 1) {
        seq_t *tmp = (seq_t*)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;                    /* unlikely fallback */
        dp = tmp;
        dp_size = n + 1;
    }

    /* initialise dp[0] = 1, others 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* main DP: for each allowed part size, accumulate counts */
    for (idx_t part = 1; part <= n; ++part) {
        if ((part % 23u) == 0u || (part % 23u) == 7u || (part % 23u) == 16u)
            continue;                          /* forbidden residue */
        for (idx_t sum = part; sum <= n; ++sum)
            dp[sum] += dp[sum - part];
    }

    return dp[n];
}
