/*
 * A104276  Number of partitions of n in which both even and odd squares occur
 *          with multiplicity 1. There is no restriction on the parts which
 *          are twice even squares.
 * https://oeis.org/A104276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n in which any odd square and any even
 *            square appear at most once; parts that are twice an even square
 *            (8k²) may be used arbitrarily many times. No other part types
 *            are allowed
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t N = n;
    /* Allocate DP array for 0..N.  calloc gives zeroes.  */
    seq_t *dp = (seq_t*)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!dp) abort();           /* fatal if allocation fails */
    dp[0] = 1;                  /* one way to form 0 */

    /* 1. unlimited parts of the form 8*k^2 (twice even squares) */
    for (idx_t k = 1;; ++k) {
        unsigned __int128 v128 = (unsigned __int128)k * k * 8;
        if (v128 > N) break;
        idx_t v = (idx_t)v128;
        for (idx_t i = v; i <= N; ++i)
            dp[i] += dp[i - v];
    }

    /* 2. odd squares (2k-1)^2 : at most one each (0-1 knapsack) */
    for (idx_t k = 1;; ++k) {
        unsigned __int128 v128 = (unsigned __int128)(2 * k - 1) * (2 * k - 1);
        if (v128 > N) break;
        idx_t v = (idx_t)v128;
        for (idx_t i = N; i >= v; --i)
            dp[i] += dp[i - v];
    }

    /* 3. even squares (2k)^2 : at most one each (0-1 knapsack) */
    for (idx_t k = 1;; ++k) {
        unsigned __int128 v128 = (unsigned __int128)(2 * k) * (2 * k);
        if (v128 > N) break;
        idx_t v = (idx_t)v128;
        for (idx_t i = N; i >= v; --i)
            dp[i] += dp[i - v];
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
