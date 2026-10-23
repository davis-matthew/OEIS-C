/*
 * A029128  Expansion of 1/((1-x)(1-x^8)(1-x^9)(1-x^11)).
 * https://oeis.org/A029128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/((1-x)(1-x^8)(1-x^9)(1-x^11))
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029128 – number of partitions of n into parts {1,8,9,11}. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑dimensional dynamic programming array that is grown as needed. */
    static seq_t *dp = NULL;      /* dp[i] = a(i) for i <= dp_size-1 */
    static size_t dp_size = 0;    /* number of valid entries in dp */

    if (dp_size <= (size_t)n)
    {
        size_t new_size = (size_t)n + 1;
        if (dp == NULL)
        {
            dp = (seq_t*)calloc(new_size, sizeof(seq_t));
            if (!dp) abort();
        }
        else
        {
            seq_t *tmp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
            if (!tmp) abort();
            dp = tmp;
        }

        /* initialise new entries to 0 (old entries are already valid) */
        for (size_t i = dp_size; i < new_size; ++i)
            dp[i] = 0;

        dp_size = new_size;
    }

    /* reset dp[0..n] to 0 and start the recurrence */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;  /* one way to make 0 using no coins */

    static const idx_t coins[4] = {1, 8, 9, 11};

    /* standard coin‑change DP loop (order of coins outer ensures
       unordered combinations). */
    for (int c = 0; c < 4; ++c)
    {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    return dp[n];
}
