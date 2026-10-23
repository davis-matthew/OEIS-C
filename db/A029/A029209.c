/*
 * A029209  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 5, 9 and 11
 * Range:     u8 n <= 101, u16 n <= 716, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent DP table.  dp[i] holds the number of partitions of i. */
    static seq_t *dp = NULL;
    static idx_t dp_len = 0;          /* number of valid entries in dp   */

    /* Parts allowed in the partitions */
    static const idx_t parts[] = {2, 5, 9, 11};
    static const size_t part_cnt = 4;

    /* If we already know the answer for this n, just return it. */
    if (n < dp_len)
        return dp[n];

    /* Build / extend the table up to n (inclusive).  We recompute from
       scratch for the new range to preserve the correct order of the
       coin‑change DP and avoid double counting. */
    idx_t new_len = n + 1;                         /* number of entries needed */
    if (dp == NULL) {
        dp = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else if (new_len > dp_len) {
        dp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
    }

    /* Initialise the whole array to zero and set dp[0] = 1 (empty sum). */
    /* It's safe to cast dp[0] to seq_t because dp[0] is used only for
       counting partitions. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i < new_len; ++i)
        dp[i] = (seq_t)0;

    /* Classic coin‑change DP: outer loop over parts, inner over sums. */
    for (size_t p = 0; p < part_cnt; ++p) {
        idx_t coin = parts[p];
        for (idx_t i = coin; i < new_len; ++i)
            dp[i] += dp[i - coin];
    }

    dp_len = new_len;
    return dp[n];
}
