/*
 * A002100  a(n) = number of partitions of n into semiprimes (more precisely,
 *          number of ways of writing n as a sum of products of 2 distinct
 *          primes).
 * https://oeis.org/A002100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count partitions of n into semiprimes (products of 2 distinct
 *            primes)
 * Range:     u8 n <= 93, u16 n <= 206, u32 n <= 537, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_semiprime_2distinct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_semiprime_2distinct.h"

seq_t a(idx_t n) {
    if (n < 6) return 0;
    
    // Allocate DP array for counting partitions
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return 0;
    
    dp[0] = 1;  // empty partition
    
    // Process each potential semiprime in order
    for (idx_t i = 6; i <= n; i++) {
        if (is_semiprime_2distinct(i)) {
            // i is a semiprime, update DP for partitions
            for (idx_t j = i; j <= n; j++) {
                dp[j] = dp[j] + dp[j - i];
            }
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
