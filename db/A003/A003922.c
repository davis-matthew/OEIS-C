/*
 * A003922  Order of universal Chevalley group B_n (5).
 * https://oeis.org/A003922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n^2) * Product_{i=1}^{n} (5^(2i) - 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    seq_t result = a000248_pow(5, n * n);
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t power_of_5 = a000248_pow(5, 2 * i);
        result *= (power_of_5 - 1);
    }
    
    return result;
}
