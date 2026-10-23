/*
 * A025149  Number of partitions of n into distinct parts >= 4.
 * https://oeis.org/A025149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts partitions of n into distinct parts ≥4 (OEIS
 *            A025149). The generating function is Π_{j≥4} (1 + x^j). Computed
 *            by dynamic programming with each part used at most once
 * Range:     u8 n <= 42, u16 n <= 99, u32 n <= 273, u64 n <= 856,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static table that keeps the DP values up to the largest n seen.
       dp[i] = number of partitions of i into distinct parts ≥4. */
    static seq_t *dp   = NULL;
    static idx_t  max_n = 0;        /* size of the table minus one */

    /* Initial allocation for n=0. */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (dp == NULL) return 0;       /* allocation failure */
        dp[0] = 1;                      /* one partition of zero */
        max_n = 0;
    }

    /* If the requested n is already within the table, simply return it. */
    if (n <= max_n)
        return dp[n];

    /* Extend the table to hold values up to n. */
    seq_t *new_dp = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
    if (new_dp == NULL) return 0;       /* allocation failure */
    dp = new_dp;

    /* Re‑compute the entire table from scratch up to the new n. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* For each possible part size j (≥4), add it to the table once.
       Updating sums in decreasing order ensures each part is used at most
       once (the “distinct” condition). */
    for (idx_t part = 4; part <= n; ++part) {
        for (idx_t sum = n; sum >= part; --sum) {
            dp[sum] += dp[sum - part];
        }
    }

    max_n = n;
    return dp[n];
}
