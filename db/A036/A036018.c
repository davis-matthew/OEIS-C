/*
 * A036018  Number of partitions of n into parts not of form 4k+2, 12k, 12k+3
 *          or 12k-3.
 * https://oeis.org/A036018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 80, u32 n <= 240, u64 n <= 797,
 *            u128 n <= 2833, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of partitions of n into parts not of form 4k+2, 12k, 12k+3 or 12k-3
   (equivalently, parts ≡ 1,4,5,7,8,11 (mod 12)) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Test whether j is an allowed part. */
static inline bool allowed_part(idx_t j)
{
    uint32_t r = (uint32_t)(j % 12);
    return r == 1 || r == 4 || r == 5 || r == 7 || r == 8 || r == 11;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Persistent buffer for dynamic programming. */
    static seq_t *dp   = NULL;
    static size_t sz   = 0;

    /* Allocate (or enlarge) the DP array. */
    size_t need = (size_t)n + 1;
    if (sz < need) {
        dp = (seq_t *)realloc(dp, need * sizeof(seq_t));
        if (!dp) exit(EXIT_FAILURE);
        sz = need;
    }

    /* Initialise the DP array. */
    for (size_t i = 0; i <= (size_t)n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Standard partition DP over allowed part values. */
    for (idx_t j = 1; j <= n; ++j) {
        if (!allowed_part(j)) continue;
        for (idx_t i = j; i <= n; ++i) {
            dp[i] += dp[i - j];
        }
    }

    return dp[(size_t)n];
}
