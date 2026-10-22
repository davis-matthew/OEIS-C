/*
 * A001722  Generalized Stirling numbers, [n+7,7]_5.
 * https://oeis.org/A001722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n+k)*binomial(k+2,
 *            2)*5^k*Stirling1(n+2, k+2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n) {
    idx_t m = n + 2;
    
    seq_t *prev = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    
    memset(prev, 0, (m + 1) * sizeof(seq_t));
    prev[0] = 1;
    
    /* Compute Stirling numbers of the first kind via recurrence */
    for (idx_t i = 1; i <= m; i++) {
        memset(curr, 0, (m + 1) * sizeof(seq_t));
        
        for (idx_t j = 1; j <= i && j <= m; j++) {
            seq_t term1 = prev[j - 1];
            seq_t term2 = ((seq_t)(i - 1)) * prev[j];
            curr[j] = term1 - term2;
        }
        
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }
    
    seq_t result = 0;
    seq_t pow5 = 1;
    
    for (idx_t k = 0; k <= n; k++) {
        int sign = ((n + k) & 1) ? -1 : 1;
        
        seq_t binom = ((seq_t)(k + 2)) * ((seq_t)(k + 1)) / 2;
        seq_t stirling = prev[k + 2];
        seq_t term = binom * pow5 * stirling;
        
        if (sign > 0) {
            result = result + term;
        } else {
            result = result - term;
        }
        
        pow5 = pow5 * 5;
    }
    
    free(prev);
    free(curr);
    
    return result;
}
