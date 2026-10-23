/*
 * A029407  Expansion of 1/((1-x^5)*(1-x^8)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 5, 8, 10 and 12
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses dynamic programming and memoises results between calls. */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Coin values */
static const idx_t COINS[4] = {5, 8, 10, 12};

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ version: use a std::vector to hold the DP table, which
       handles construction/destruction of arbitrary‑precision objects. */
    static std::vector<seq_t> dp;
    const size_t sz = (size_t)(n + 1);
    if (dp.size() < sz)
    {
        /* Resize and initialise to 0. */
        dp.assign(sz, 0);
        dp[0] = 1;
        /* Compute partitions using the coin‑change DP. */
        for (size_t ci = 0; ci < 4; ++ci)
        {
            const idx_t coin = COINS[ci];
            for (size_t i = coin; i < sz; ++i)
                dp[i] += dp[i - coin];
        }
    }
    return dp[n];
#else
    /* C version: use raw allocation (realloc) and unsigned integer types. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;  /* One more than the highest index stored. */

    if (n >= dp_size)
    {
        const size_t new_size = (size_t)(n + 1);
        seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!new_dp)
            abort();                 /* Out of memory. */
        dp = new_dp;

        /* Initialise all new entries to 0. */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = 0;
        dp[0] = 1;                  /* Base case: one partition of 0. */

        /* DP update for all coins. */
        for (size_t ci = 0; ci < 4; ++ci)
        {
            const idx_t coin = COINS[ci];
            for (idx_t i = coin; i <= n; ++i)
                dp[i] += dp[i - coin];
        }

        dp_size = n + 1;            /* Update maximum computed index. */
    }
    return dp[n];
#endif
}
