/*
 * A029372  Expansion of 1/((1-x^4)*(1-x^8)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 4, 8, 10, and 11
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: classic coin‑change DP, computing a(n) = Σ a(n‑coin) for each
   coin value (4,8,10,11).  Complexity O(n) per call. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Coin values as unsigned indices */
    const size_t coins[4] = {4, 8, 10, 11};

    /* Temporary DP array */
    size_t N = (size_t)n;
    seq_t *dp;

#ifdef __cplusplus
    /* In C++ use value‑initialised dynamic array */
    dp = new seq_t[N + 1]();
#else
    /* In C use calloc to zero‑initialise the array */
    dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
#endif
    if (!dp) return 0;      /* allocation failed – return 0 as safe value */

    dp[0] = 1;  /* base case: one way to make 0 */

    /* DP: for each coin, add ways to reach each amount */
    for (int c = 0; c < 4; ++c)
    {
        size_t coin = coins[c];
        for (size_t i = coin; i <= N; ++i)
        {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[N];

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
