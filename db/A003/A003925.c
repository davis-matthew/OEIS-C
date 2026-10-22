/*
 * A003925  Order of universal Chevalley group B_n (8).
 * https://oeis.org/A003925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n^2) * Product_{i=1}^{n} (8^(2i) - 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"


seq_t a(idx_t n) {
    // Compute 8^(n^2)
    idx_t n_sq = n * n;
    seq_t result = a000248_pow((seq_t)8, n_sq);
    
    // Compute Product_{i=1}^{n} (8^(2i) - 1)
    for (idx_t i = 1; i <= n; i++) {
        seq_t power = a000248_pow((seq_t)8, 2 * i);
        result *= (power - 1);
    }
    
    return result;
}
