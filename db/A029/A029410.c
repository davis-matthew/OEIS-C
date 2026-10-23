/*
 * A029410  Expansion of 1/((1-x^5)*(1-x^9)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A029410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029410:  a(n) = number of solutions of 5*a+9*b+10*c+12*d = n,
   counted without ordering. This is the partition function with parts
   5, 9, 10, and 12.  The function uses a simple coin‑change DP and
   recomputes the whole table for each call (O(n) time, O(n) space). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert n to a usable size.  n is guaranteed to fit into a size_t
       in the test harness (n ≤ 10 000). */
    size_t N = (size_t)n;

    /* dp[i] counts the number of solutions for value i. */
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (dp == NULL) return 0;         /* fallback on allocation failure */

    dp[0] = 1;                         /* one way to make 0 (all zeros) */

    /* Coin weights.  The outer loop over coins ensures that each
       combination is counted only once regardless of order. */
    const idx_t coins[] = {5, 9, 10, 12};
    const size_t coin_cnt = sizeof(coins) / sizeof(coins[0]);

    for (size_t ci = 0; ci < coin_cnt; ++ci) {
        idx_t w = coins[ci];
        /* Inner loop iterates forward; we cast w to size_t for the
           bounds check. */
        for (size_t i = (size_t)w; i <= N; ++i) {
            /* dp[i] += dp[i-w];  (no overflow in seq_t; addition only) */
            dp[i] += dp[i - (size_t)w];
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
