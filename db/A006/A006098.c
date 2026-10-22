/*
 * A006098  Gaussian binomial coefficient [ 2n,n ] for q=2.
 * https://oeis.org/A006098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [2n,n]_2, the Gaussian binomial coefficient
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"

seq_t a(idx_t n) {
    seq_t result = (seq_t)1;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t num = A001832_pow2(2*n - i + 1) - 1;
        seq_t denom = A001832_pow2(i) - 1;
        
        result *= num;
        result /= denom;
    }
    
    return result;
}
