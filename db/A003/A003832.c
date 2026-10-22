/*
 * A003832  Order of universal Chevalley group D_n (5).
 * https://oeis.org/A003832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n(n-1)) * (5^n - 1) * ∏_{i=1}^{n-1} (5^(2i) - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t five = (seq_t)5;
    
    result *= a000248_pow(five, n * (n - 1));
    result *= (a000248_pow(five, n) - 1);
    
    for (idx_t i = 1; i < n; i++) {
        result *= (a000248_pow(five, 2 * i) - 1);
    }
    
    return result;
}
