/*
 * A007294  Number of partitions of n into nonzero triangular numbers.
 * https://oeis.org/A007294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 145, u32 n <= 594, u64 n <= 2919,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partitions of n into triangular numbers k*(k+1)/2, computed via dynamic programming. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    // Allocate and initialize DP array
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    memset(dp, 0, (n + 1) * sizeof(seq_t));
    dp[0] = 1;
    
    // For each triangular number
    for (idx_t k = 1; ; k++) {
        idx_t tri = k * (k + 1) / 2;
        if (tri > n) break;
        
        // Update dp[i] by adding partitions that include this triangular number
        for (idx_t i = tri; i <= n; i++) {
            dp[i] += dp[i - tri];
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    
    return result;
}
