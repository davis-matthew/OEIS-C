/*
 * A006129  a(0), a(1), a(2), ... satisfy Sum_{k=0..n} a(k)*binomial(n,k) =
 *          2^binomial(n,2), for n >= 0.
 * https://oeis.org/A006129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k)*binomial(n, k)*2^binomial(k, 2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binom, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pw2.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t k = 0; k <= n; k++) {
        seq_t binom_n_k = binom(n, k);
        
        idx_t binom_k_2 = (k * (k - 1)) / 2;
        seq_t pow2_binom = pw2(binom_k_2);
        
        seq_t term = binom_n_k * pow2_binom;
        
        if ((n - k) % 2 == 0) {
            result = result + term;
        } else {
            result = result - term;
        }
    }
    
    return result;
}
