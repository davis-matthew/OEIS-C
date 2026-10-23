/*
 * A193748  Number of partitions of n into parts that are squares or
 *          triangular numbers (A005214).
 * https://oeis.org/A193748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 85, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the number of partitions of n into parts that are squares
    or triangular numbers.  The algorithm:
        1.  Build a list of all distinct integers ≤ n that are either
            a perfect square or a triangular number.
        2.  Use a classic 1‑dimensional DP (unbounded knapsack) where
            dp[i] = number of partitions of i using the allowed parts.
            dp[0] = 1; for each part p, for i = p..n:
                dp[i] += dp[i‑p].
    Complexity:  O(n · (number of parts))   ≤ O(n·250) for n ≤ 10000.
    Memory:      O(n)                          (dp array only). */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- Build the set of admissible parts ---------- */
    idx_t parts[512];
    size_t part_cnt = 0;
    for (idx_t k = 1; ; ++k)          /* squares */
    {
        idx_t sq = k * k;
        if (sq > n) break;
        /* check for duplicates */
        bool found = false;
        for (size_t i = 0; i < part_cnt; ++i)
            if (parts[i] == sq) { found = true; break; }
        if (!found) parts[part_cnt++] = sq;
    }
    for (idx_t k = 1; ; ++k)          /* triangular numbers */
    {
        idx_t tri = k * (k + 1) / 2;
        if (tri > n) break;
        bool found = false;
        for (size_t i = 0; i < part_cnt; ++i)
            if (parts[i] == tri) { found = true; break; }
        if (!found) parts[part_cnt++] = tri;
    }

    /* ---------- DP array ---------- */
    /* cast n to size_t for allocation - n is guaranteed nonnegative */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) return 0;            /* allocation failure (rare) */
    dp[0] = 1;

    /* ---------- Unbounded knapsack ---------- */
    for (size_t pi = 0; pi < part_cnt; ++pi)
    {
        idx_t p = parts[pi];
        size_t ps = (size_t)p;
        for (size_t i = ps; i < sz; ++i)
            dp[i] += dp[i - ps];
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
