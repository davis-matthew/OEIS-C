/*
 * A029262  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of partitions of n into parts 3, 4, 8, and 9.
 *            It is computed by the classic unbounded‑knapsack (coin‑change)
 *            DP: dp[0] = 1; for each coin in {3,4,8,9} for s = coin to n
 *            dp[s] += dp[s-coin];
 * Range:     u8 n <= 98, u16 n <= 685, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is thread‑local static; it recomputes all values
   up to the requested n each call, which keeps the implementation
   simple and guarantees correctness for any order of calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;      /* array of values a[0..max_n] */
    static idx_t cap = 0;         /* current capacity of dp */

    /* Ensure dp has room for n+1 elements. */
    if (n + 1 > cap) {
        seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!new_dp)   /* allocation failure – return 0 as safe default */
            return (seq_t)0;
        dp = new_dp;
        cap = n + 1;
    }

    /* Initialize dp[0] = 1, rest 0 up to n. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Unbounded‑knapsack: count combinations (order‑irrelevant). */
    static const idx_t coins[4] = {3, 4, 8, 9};
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t s = coin; s <= n; ++s) {
            dp[s] += dp[s - coin];
        }
    }

    return dp[n];
}
