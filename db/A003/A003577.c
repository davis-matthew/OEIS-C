/*
 * A003577  Dowling numbers: e.g.f. exp(x + (exp(b*x)-1)/b) with b=5.
 * https://oeis.org/A003577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+1) = a(n) + Sum_{k=0}^{n} C(n,k) * a(k) * 5^{n-k}, a(0) = 1 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t *vals = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    vals[0] = 1;
    
    seq_t pow5_i = 1;
    
    for (idx_t i = 0; i < n; i++) {
        seq_t sum = vals[i];
        seq_t current_power = pow5_i;
        seq_t binom = 1;
        
        for (idx_t k = 0; k <= i; k++) {
            sum += binom * vals[k] * current_power;
            
            if (k < i) {
                binom = binom * (i - k) / (k + 1);
                current_power = current_power / 5;
            }
        }
        
        vals[i + 1] = sum;
        pow5_i *= 5;
    }
    
    seq_t result = vals[n];
    free(vals);
    return result;
}
