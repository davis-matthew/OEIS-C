/*
 * A089197  Nonadjacent Fibonacci currency: number of ways to make change for
 *          n units in a currency system with coins of value 1, 2, 5, 13, 34,
 *          89, ..., Fibonacci(2k-1).
 * https://oeis.org/A089197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of ways to make change for n units using coins of
 *            values F_{2k-1} = 1, 2, 5, 13, 34, 89, ... (odd indexed
 *            Fibonacci numbers), i.e. the number of partitions of n with
 *            parts in that set
 * Range:     u8 n <= 47, u16 n <= 247, u32 n <= 2302, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;           /* allocation failure → zero (unlikely) */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Generate odd‑indexed Fibonacci coins up to n. */
    seq_t coins[64];
    int   coin_count = 0;

    seq_t prev = (seq_t)0,
          curr = (seq_t)1;
    idx_t fib_idx = 1;            /* F_1 = 1 */

    while (curr <= n) {
        if ((fib_idx & 1U) != 0)          /* odd index -> coin */
            coins[coin_count++] = curr;

        /* Next Fibonacci number */
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
        ++fib_idx;
    }

    /* Unbounded coin change DP: for each coin, add ways to make all sums. */
    for (int c = 0; c < coin_count; ++c) {
        seq_t coin = coins[c];
        for (idx_t j = (idx_t)coin; j <= n; ++j) {
            dp[j] += dp[j - (idx_t)coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
