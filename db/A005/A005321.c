/*
 * A005321  Upper triangular n X n (0,1)-matrices with no zero rows or
 *          columns.
 * https://oeis.org/A005321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + Sum_{k=2..n} binomial(n,k)*Sum{i=2..k}
 *            (-1)^i*Product_{j=i+1..k} (2^j - 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    
    for (idx_t k = 2; k <= n; k++) {
        seq_t inner_sum = 0;
        seq_t product = 1;
        
        for (idx_t i = k; i >= 2; i--) {
            int sign = (i % 2 == 0) ? 1 : -1;
            if (sign == 1) {
                inner_sum += product;
            } else {
                inner_sum -= product;
            }
            
            if (i > 2) {
                product *= ((seq_t)1 << (unsigned int)i) - 1;
            }
        }
        
        seq_t binom_coeff = binom(n, k);
        result += binom_coeff * inner_sum;
    }
    
    return result;
}
