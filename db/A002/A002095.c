/*
 * A002095  Number of partitions of n into nonprime parts.
 * https://oeis.org/A002095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Number of partitions of n into nonprime parts; G.f.:
 *            Product_{i>0} (1-x^prime(i))/(1-x^i)
 * Range:     u8 n <= 29, u16 n <= 75, u32 n <= 210, u64 n <= 647,
 *            u128 n <= 2145, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    /* Sieve of Eratosthenes to find all primes up to n */
    bool *is_prime = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    if (!is_prime) return 0;
    
    memset(is_prime, true, (size_t)(n + 1) * sizeof(bool));
    is_prime[0] = false;
    is_prime[1] = false;
    
    for (idx_t i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    /* Allocate DP array */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) {
        free(is_prime);
        return 0;
    }
    
    /* Initialize DP array */
    for (idx_t i = 0; i <= n; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;
    
    /* Count partitions using non-prime parts */
    for (idx_t p = 1; p <= n; p++) {
        if (p == 1 || !is_prime[p]) {
            for (idx_t i = p; i <= n; i++) {
                dp[i] += dp[i - p];
            }
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    free(is_prime);
    return result;
}
