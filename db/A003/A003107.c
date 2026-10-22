/*
 * A003107  Number of partitions of n into Fibonacci parts (with a single type
 *          of 1).
 * https://oeis.org/A003107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: Product_{i>=2} 1/(1-x^Fibonacci(i)); coin-change DP over
 *            the distinct Fibonacci parts 1,2,3,5,8,
 * Range:     u8 n <= 24, u16 n <= 92, u32 n <= 519, u64 n <= 5071,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;

    idx_t fib_a = 1, fib_b = 1;
    while (fib_b <= n) {
        for (idx_t i = fib_b; i <= n; i++) {
            dp[i] += dp[i - fib_b];
        }
        idx_t fib_c = fib_a + fib_b;
        if (fib_c < fib_b) break;
        fib_a = fib_b;
        fib_b = fib_c;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
