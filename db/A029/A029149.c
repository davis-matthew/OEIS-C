/*
 * A029149  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^5)*(1-x^12)).
 * https://oeis.org/A029149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 510, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A029149: Partitions of n into parts 2, 3, 5, and 12
 * Formula: a(n) = number of non‑negative integer solutions of
 *   2x + 3y + 5z + 12w = n
 * Computation: dynamic programming (coin change) over the set {2,3,5,12}.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for allocation */
    static seq_t *dp = NULL;      /* array of results */
    static size_t cap = 0;        /* size of allocated array */

    /* reallocate if necessary (C or C++ compatible) */
    if ((size_t)(n + 1) > cap) {
#ifdef __cplusplus
        seq_t *newdp = new seq_t[n + 1];
        if (dp) delete[] dp;
#else
        seq_t *newdp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
#endif
        dp   = newdp;
        cap  = n + 1;
    }

    /* initialise all entries to 0, then set base case */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* allowed parts in the partition */
    const idx_t parts[4] = { 2, 3, 5, 12 };

    /* dynamic programming: add contributions for each part */
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        for (idx_t j = part; j <= n; ++j) {
            dp[j] += dp[j - part];
        }
    }

    return dp[n];
}
