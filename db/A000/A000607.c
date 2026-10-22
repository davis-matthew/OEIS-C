/*
 * A000607  Number of partitions of n into prime parts.
 * https://oeis.org/A000607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   G.f. 1/Product_{k>=1} (1-x^prime(k)) computed via partition DP
 * Range:     u8 n <= 38, u16 n <= 107, u32 n <= 353, u64 n <= 1327,
 *            u128 n <= 5442, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>


seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;
    
    for (idx_t i = 0; i <= n; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;
    
    for (idx_t p = 2; p <= n; p++) {
        bool is_prime = true;
        if (p > 2 && p % 2 == 0) {
            is_prime = false;
        } else {
            for (idx_t i = 3; i * i <= p; i += 2) {
                if (p % i == 0) {
                    is_prime = false;
                    break;
                }
            }
        }
        
        if (is_prime) {
            for (idx_t i = p; i <= n; i++) {
                dp[i] += dp[i - p];
            }
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
