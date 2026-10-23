/*
 * A025819  Expansion of 1/((1-x^2)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A025819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 7, and 11
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Computation: dynamic programming (order‑independent coin change) */
seq_t a(idx_t n)
{
    /* static cache of computed values up to current_max */
    static seq_t *dp = NULL;
    static size_t dp_len = 0;
    static idx_t current_max = 0;

    /* If we already have the value, return it. */
    if (n <= current_max && dp != NULL)
        return dp[(size_t)n];

    /* Allocate / enlarge the array to hold values up to n. */
    if (dp_len < (size_t)(n + 1))
    {
        size_t new_len = dp_len ? dp_len : 16;
        while (new_len < (size_t)(n + 1))
            new_len <<= 1;
        dp = (seq_t*)realloc(dp, new_len * sizeof(seq_t));
        dp_len = new_len;
    }

    /* Initialize the part of the array that will be used. */
    for (size_t i = 0; i <= (size_t)n; ++i)
        dp[i] = 0;

    /* Base case: one way to form 0. */
    dp[0] = 1;

    /* The three allowed part sizes. */
    const idx_t parts[3] = {2, 7, 11};

    /* Classic coin‑change DP: for each part, accumulate contributions. */
    for (int p = 0; p < 3; ++p)
    {
        idx_t coin = parts[p];
        for (idx_t i = coin; i <= n; ++i)
            dp[(size_t)i] += dp[(size_t)(i - coin)];
    }

    current_max = n;
    return dp[(size_t)n];
}
