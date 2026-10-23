/*
 * A136343  Number of partitions of n such that each summand is greater than
 *          or equal to the sum of the next two summands.
 * https://oeis.org/A136343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of all integers m (0 ≤ m ≤ n) using
 *            parts equal to (Fibonacci(k) – 1) for all k ≥ 3
 * Range:     u8 n <= 31, u16 n <= 126, u32 n <= 741, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Generate the coin values: f_k - 1 for Fibonacci numbers f_k ≥ 3.
       The largest such coin used will be ≤ n.               */
    idx_t fib_prev = 1, fib_cur = 1;   /* Fibonacci numbers f(1)=1, f(2)=1 */
    idx_t fib_next;
    idx_t coin;
    idx_t coins_capacity = 20;
    idx_t *coins = (idx_t*)malloc(coins_capacity * sizeof(idx_t));
    idx_t coins_cnt = 0;

    while (1) {
        fib_next = fib_prev + fib_cur;   /* next Fibonacci number */
        fib_prev = fib_cur;
        fib_cur = fib_next;
        if (fib_cur <= 2) continue;      /* skip 1,1,2 */
        coin = fib_cur - 1;              /* (f_k - 1) value */
        if (coin > n) break;
        if (coins_cnt == coins_capacity) {
            coins_capacity *= 2;
            coins = (idx_t*)realloc(coins, coins_capacity * sizeof(idx_t));
        }
        coins[coins_cnt++] = coin;
    }

    /* DP array: dp[i] = number of partitions of i using the coin set.
       Allocate an array of seq_t of size n+1. */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Unbounded knapsack DP */
    for (idx_t k = 0; k < coins_cnt; ++k) {
        idx_t v = coins[k];
        for (idx_t i = v; i <= n; ++i) {
            dp[i] += dp[i - v];
        }
    }

    /* Sum dp[0] .. dp[n] to get the final answer. */
    seq_t ans = (seq_t)0;
    for (idx_t i = 0; i <= n; ++i) {
        ans += dp[i];
    }

    /* Clean up */
    free(dp);
    free(coins);

    return ans;
}
