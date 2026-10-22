/*
 * A002124  Number of compositions of n into a sum of odd primes.
 * https://oeis.org/A002124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 48, u32 n <= 91, u64 n <= 176,
 *            u128 n <= 345, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic programming: a(0)=1, a(1)=a(2)=0;
   for n >= 3, a(n) = Sum_{odd primes p in [3, n]} a(n-p) */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n <= 2) return (seq_t)0;
    
    seq_t *dp = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;
    
    bool *is_prime = (bool *)malloc(((size_t)n + 1) * sizeof(bool));
    if (!is_prime) {
        free(dp);
        return (seq_t)0;
    }
    
    memset(is_prime, 1, ((size_t)n + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = 0;
    
    for (uint64_t i = 2; i * i <= (uint64_t)n; i++) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= (uint64_t)n; j += i) {
                is_prime[j] = 0;
            }
        }
    }
    
    memset(dp, 0, ((size_t)n + 1) * sizeof(seq_t));
    dp[0] = (seq_t)1;
    
    for (idx_t i = 3; i <= n; i++) {
        for (idx_t p = 3; p <= i; p++) {
            if (is_prime[p]) {
                dp[i] += dp[i - p];
            }
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    free(is_prime);
    return result;
}
