/*
 * A000823  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^(n+1) - n - 1) + 2^(2^n) - 2^(2^n - n - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a000740_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000740_pow2.h"

seq_t a(idx_t n) {
    idx_t pow2_n = 1ULL << n;
    idx_t pow2_n_plus_1 = 1ULL << (n + 1);
    
    idx_t exp1 = pow2_n_plus_1 - n - 1;
    idx_t exp2 = pow2_n;
    idx_t exp3 = pow2_n - n - 1;
    
    seq_t term1 = a000740_pow2(exp1);
    seq_t term2 = a000740_pow2(exp2);
    seq_t term3 = a000740_pow2(exp3);
    
    return term1 + term2 - term3;
}
