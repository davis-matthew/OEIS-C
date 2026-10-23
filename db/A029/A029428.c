/*
 * A029428  Expansion of 1/((1-x^6)(1-x^9)(1-x^10)(1-x^11)).
 * https://oeis.org/A029428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of (unordered) ways to write n as a sum of 6, 9,
 *            10 and 11 (each can be used any number of times)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* cache of coefficients */
static idx_t  dp_size = 0;    /* number of elements stored (dp[0..dp_size-1]) */

static const idx_t coin_values[4] = {6, 9, 10, 11};

/* Recompute all coefficients up to index n.  This keeps the DP
   simple and correct for every call, regardless of the order
   of requests.  The cost is O(n) per request that increases the
   maximum n, which is fine for n ≤ 10000. */
static void recompute_dp(idx_t n)
{
    /* Allocate or resize the array to accommodate indices up to n. */
    idx_t new_size = n + 1;
    if (dp == NULL)
        dp = (seq_t *)malloc(new_size * sizeof(seq_t));
    else
        dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    dp_size = new_size;

    /* Initialize all entries to zero, except dp[0] = 1. */
    dp[0] = 1;
    for (idx_t i = 1; i < dp_size; ++i)
        dp[i] = 0;

    /* Classic coin‑change DP: iterate coins first to count
       combinations without considering ordering. */
    for (size_t ci = 0; ci < 4; ++ci) {
        idx_t coin = coin_values[ci];
        for (idx_t i = coin; i < dp_size; ++i)
            dp[i] += dp[i - coin];
    }
}

seq_t a(idx_t n)
{
    /* Grow or recompute the DP table as needed. */
    if (dp == NULL || n >= dp_size)
        recompute_dp(n);
    return dp[n];
}
