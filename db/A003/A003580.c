/*
 * A003580  Dowling numbers: e.g.f. exp(x + (exp(b*x)-1)/b) with b=8.
 * https://oeis.org/A003580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n) + Sum_{k=0}^{n} C(n,k) * 8^k * a(n-k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    if (n == 0) return 1;
    
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;
    
    dp[0] = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t sum = 0;
        seq_t power8 = 1;
        
        for (idx_t k = 0; k < i; k++) {
            // Compute C(i-1, k)
            seq_t binom = 1;
            for (idx_t j = 0; j < k; j++) {
                binom = binom * (i - 1 - j) / (j + 1);
            }
            
            sum += binom * power8 * dp[i - 1 - k];
            power8 *= 8;
        }
        
        dp[i] = dp[i-1] + sum;
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
