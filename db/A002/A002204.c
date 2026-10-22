/*
 * A002204  An ill-conditioned determinant.
 * https://oeis.org/A002204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2n-2,n-2)*binomial(2n-2,n-1)*(2n-1)*a(n-1) with
 *            a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   binom_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_helper.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    
    seq_t result = (seq_t)1;
    
    for (idx_t k = 2; k <= n; k++) {
        seq_t c1 = binom_helper(2*k - 2, k - 2);
        seq_t c2 = binom_helper(2*k - 2, k - 1);
        seq_t factor = (seq_t)(2*k - 1);
        
        result = result * c1 * c2 * factor;
    }
    
    return result;
}
