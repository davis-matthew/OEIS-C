/*
 * A029095  Expansion of 1/((1-x)*(1-x^5)*(1-x^7)*(1-x^12)).
 * https://oeis.org/A029095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 5, 7, 12
 *            Computation: dynamic programming counting coin sums with parts
 *            {1,5,7,12} The algorithm runs in O(4·n) time and O(n) memory
 * Range:     u8 n <= 74, u16 n <= 536, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t sz = (size_t)(n + 1);
    /* Allocate an array to hold the coin‑change counts.
       The array is initialized to zero (except dp[0] which is set to 1). */
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) /* out of memory – return 0 as a safe fallback */
        return (seq_t)0;

    dp[0] = (seq_t)1;
    for (size_t i = 1; i < sz; ++i)
        dp[i] = (seq_t)0;

    /* Parts available for the partitions. */
    const int parts[4] = {1, 5, 7, 12};

    for (int p = 0; p < 4; ++p) {
        int part = parts[p];
        for (size_t i = (size_t)part; i < sz; ++i) {
            dp[i] = dp[i] + dp[i - part];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
