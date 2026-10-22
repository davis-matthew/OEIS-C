/*
 * A000586  Number of partitions of n into distinct primes.
 * https://oeis.org/A000586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Product_{k>=1} (1+x^prime(k)), counting partitions of n into
 *            distinct primes
 * Range:     u8 n <= 106, u16 n <= 309, u32 n <= 1038, u64 n <= 3874,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>


static bool is_prime(uint64_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= p; i += 2) {
        if (p % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    uint64_t nn = (uint64_t)n;
    seq_t *dp = (seq_t *)calloc(nn + 1, sizeof(seq_t));
    dp[0] = 1;
    
    for (uint64_t p = 2; p <= nn; p++) {
        if (is_prime(p)) {
            for (uint64_t i = nn; i >= p; i--) {
                dp[i] += dp[i - p];
            }
        }
    }
    
    seq_t result = dp[nn];
    free(dp);
    return result;
}
