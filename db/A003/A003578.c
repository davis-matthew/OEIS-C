/*
 * A003578  Dowling numbers: e.g.f. exp(x + (exp(b*x) - 1)/b), with b=6.
 * https://oeis.org/A003578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n) + sum_{k=0}^{n} C(n,k) * a(k) * 6^(n-k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
        
    if (n == 0) return 1;
    
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    dp[0] = 1;
    
    for (idx_t m = 0; m < n; m++) {
        seq_t sum = 0;
        
        for (idx_t k = 0; k <= m; k++) {
            /* Compute C(m, k) */
            seq_t binom = 1;
            for (idx_t i = 1; i <= k; i++) {
                binom = binom * (m - i + 1) / i;
            }
            
            /* Compute 6^(m-k) */
            seq_t power = 1;
            for (idx_t i = m; i > k; i--) {
                power *= 6;
            }
            
            sum += binom * dp[k] * power;
        }
        
        dp[m + 1] = dp[m] + sum;
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
