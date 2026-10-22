/*
 * A000557  Expansion of e.g.f. 1/(1 - 2*sinh(x)).
 * https://oeis.org/A000557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} Sum_{j=0..k} (-1)^j*binomial(k,j)*(k-2*j)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   compute_binomial, compute_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_binomial.h"
#include "../utils/compute_power.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    
    for (idx_t k = 0; k <= n; k++) {
        for (idx_t j = 0; j <= k; j++) {
            seq_t binom = compute_binomial(k, j);
            seq_t base = (seq_t)k - 2 * (seq_t)j;
            seq_t power = compute_power(base, n);
            seq_t term = binom * power;
            
            if (j % 2 == 0) {
                result += term;
            } else {
                result -= term;
            }
        }
    }
    
    return result;
}
