/*
 * A007000  Number of partitions of n into Fibonacci parts (with 2 types of
 *          1).
 * https://oeis.org/A007000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = partitions of n into Fibonacci numbers with 2 types of 1,
 *            via DP from G.f.: 1/Product_{j>=1}(1-x^Fibonacci(j))
 * Range:     u8 n <= 14, u16 n <= 55, u32 n <= 314, u64 n <= 3092,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    seq_t *dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    dp[0] = 1;
    
    // Process 1 twice (two types)
    for (idx_t j = 1; j <= n; j++) {
        dp[j] += dp[j - 1];
    }
    for (idx_t j = 1; j <= n; j++) {
        dp[j] += dp[j - 1];
    }
    
    // Generate and process Fibonacci numbers >= 2
    idx_t fib_prev = 1, fib_curr = 2;
    while (fib_curr <= n) {
        for (idx_t j = fib_curr; j <= n; j++) {
            dp[j] += dp[j - fib_curr];
        }
        idx_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
