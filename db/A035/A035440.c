/*
 * A035440  Number of partitions of n into parts 7k+5 or 7k+6.
 * https://oeis.org/A035440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 184, u32 n <= 501, u64 n <= 1553,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035440
 * Number of partitions of n into parts 7k+5 or 7k+6.
 * Formula (generating function):
 *   G(q) = ∏_{k≥0} 1/((1-q^(7k+5))·(1-q^(7k+6)))
 * Computation: dynamic programming over all part sizes in increasing order.
 *  a(n) = number of ways to write n as a sum of numbers of the form 5,6,12,13,19,20,...
 *  Uses an internal static cache so that repeated calls are efficient.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static buffer that grows as needed.  It always contains dp[0..dp_capacity-1]. */
    static seq_t *dp = NULL;
    static idx_t dp_capacity = 0;

    /* Ensure the buffer is large enough for indices 0 … n. */
    if (dp_capacity <= n)
    {
        idx_t new_capacity = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_capacity * sizeof(seq_t));
        if (!new_dp) return 0;           /* allocation failed – give up */
        dp = new_dp;
        dp_capacity = new_capacity;
    }

    /* Re‑initialize dp[0..n] to zero and set dp[0] = 1 (empty partition). */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Add parts congruent to 5 (5,12,19,…) */
    for (idx_t p = 5; p <= n; p += 7)
    {
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    /* Add parts congruent to 6 (6,13,20,…) */
    for (idx_t p = 6; p <= n; p += 7)
    {
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];
    }

    return dp[n];
}
