/*
 * A001308  Order of Chevalley group D_n (2).
 * https://oeis.org/A001308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n*(n-1)) * (2^n - 1) * Product_{i=1..n-1} (2^(2*i) -
 *            1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   a000740_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000740_pow2.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    
    idx_t exp1 = n * (n - 1);
    result = a000740_pow2(exp1);
    
    seq_t term = a000740_pow2(n) - 1;
    result = result * term;
    
    for (idx_t i = 1; i < n; i++) {
        seq_t power = a000740_pow2(2 * i) - 1;
        result = result * power;
    }
    
    return result;
}
