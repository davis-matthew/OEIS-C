/*
 * A087154  Number of partitions of n into distinct nonsquares.
 * https://oeis.org/A087154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct nonsquares
 * Range:     u8 n <= 40, u16 n <= 100, u32 n <= 287, u64 n <= 911,
 *            u128 n <= 3128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming.  For each integer m from 1 to n that
   is not a perfect square, we add it at most once to every partial sum that
   does not exceed n.  dp[s] finally contains the desired count for sum s. */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <algorithm>
#endif

seq_t a(idx_t n)
{
    /* allocate the DP array holding seq_t values */
    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[n + 1];
    std::fill(dp, dp + n + 1, 0);
#else
    dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;               /* allocation failed */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
#endif

    /* dp[0] = 1 : empty partition */
    dp[0] = 1;

    /* pre‑compute which numbers up to n are perfect squares */
    bool *isSq;
#ifdef __cplusplus
    isSq = new bool[n + 1];
    std::fill(isSq, isSq + n + 1, false);
#else
    isSq = (bool *)malloc((n + 1) * sizeof(bool));
    if (!isSq) { free(dp); return 0; }
    for (idx_t i = 0; i <= n; ++i) isSq[i] = false;
#endif
    for (idx_t k = 1;; ++k) {
        idx_t sq = k * k;
        if (sq > n) break;
        isSq[sq] = true;
    }

    /* dynamic programming: each non‑square can be used at most once */
    for (idx_t part = 1; part <= n; ++part) {
        if (isSq[part]) continue;      /* skip squares */
        for (idx_t s = n; s >= part; --s) {
            dp[s] += dp[s - part];
        }
    }

    seq_t result = dp[n];

#ifdef __cplusplus
    delete[] isSq;
    delete[] dp;
#else
    free(isSq);
    free(dp);
#endif
    return result;
}
