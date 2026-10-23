/*
 * A015753  Number of partitions of n into distinct parts, none being 6.
 * https://oeis.org/A015753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts, none equal
 *            to 6
 * Range:     u8 n <= 31, u16 n <= 83, u32 n <= 247, u64 n <= 811,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function recomputes the value from scratch for each call. */
seq_t a(idx_t n)
{
    /* 10001 is enough for all test data (n ≤ 10000).  Large enough so that
       memory usage stays well within limits. */
    static seq_t dp[10001];

    /* Guard against an out‑of‑range request; the caller should not request
       n ≥ 10001.  Returning 0 signals an error in that unlikely case. */
    if (n >= 10001)
        return (seq_t)0;

    /* Initialise dp[0] and zero the rest of the array.  We explicitly
       initialise each element to avoid relying on default constructors
       when building with arbitrary‑precision classes. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Classic 0‑1 knapsack DP: each part used at most once. */
    for (idx_t part = 1; part <= n; ++part) {
        if (part == 6)
            continue; /* part 6 is forbidden */

        for (idx_t sum = n; sum >= part; --sum) {
            dp[sum] += dp[sum - part];
        }
    }

    return dp[n];
}
