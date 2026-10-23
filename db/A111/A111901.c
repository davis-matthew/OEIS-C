/*
 * A111901  Number of partitions of n into parts that are primes or squares of
 *          primes.
 * https://oeis.org/A111901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts that are primes or
 *            squares of primes
 * Range:     u8 n <= 28, u16 n <= 78, u32 n <= 261, u64 n <= 1010,
 *            u128 n <= 4340, big any n
 * Helpers:   simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/simple_sieve.h"

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return 0;                /* allocation failure */

    dp[0] = 1;
    for (size_t i = 1; i < len; ++i)
        dp[i] = 0;

    /* Obtain all primes up to n */
    uint32_t *primes = NULL;
    size_t prime_count = 0;
    simple_sieve((uint32_t)n, &primes, &prime_count);

    /* Partitions using prime parts */
    for (size_t pi = 0; pi < prime_count; ++pi) {
        size_t coin = (size_t)primes[pi];
        if (coin > n) continue;
        for (size_t i = coin; i <= (size_t)n; ++i)
            dp[i] += dp[i - coin];
    }

    /* Partitions using square-of-prime parts */
    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint64_t p  = (uint64_t)primes[pi];
        uint64_t sq = p * p;
        if (sq > (uint64_t)n) break;
        size_t coin = (size_t)sq;
        for (size_t i = coin; i <= (size_t)n; ++i)
            dp[i] += dp[i - coin];
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    free(primes);
    return result;
}
