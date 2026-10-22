/*
 * A000450  a(n) = A058057(n,n-4).
 * https://oeis.org/A000450
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=4..n} (-1)^k * (n-k)! * binomial(k,4) *
 *            binomial(2*n-k,k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"


seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    seq_t factorial = 1;
    
    // Compute (n-4)! first
    for (idx_t i = 1; i <= n - 4; i++) {
        factorial *= (seq_t)i;
    }
    
    for (idx_t k = 4; k <= n; k++) {
        // At this point, factorial = (n-k)!
        
        // Compute binomial(k, 4)
        seq_t binom_k_4 = binomial(k, 4);
        
        // Compute binomial(2*n-k, k)
        seq_t binom_2n_minus_k_k = binomial((idx_t)(2*n - k), k);
        
        // Compute the term
        seq_t term = factorial * binom_k_4 * binom_2n_minus_k_k;
        
        // Add or subtract based on (-1)^k
        if (k % 2 == 0) {
            pos += term;
        } else {
            neg += term;
        }
        
        // Update factorial for next iteration
        if (k < n) {
            factorial /= (seq_t)(n - k);
        }
    }
    
    return pos - neg;
}
