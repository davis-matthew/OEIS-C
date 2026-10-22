/*
 * A003791  Order of universal Chevalley group A_n (8).
 * https://oeis.org/A003791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n(n+1)/2) * ∏_{i=2}^{n+1} (8^i - 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    seq_t q = 8;
    
    // Compute q^(n(n+1)/2)
    idx_t exp = n * (n + 1) / 2;
    seq_t result = a000248_pow(q, exp);
    
    // Multiply by ∏_{i=2}^{n+1} (q^i - 1)
    seq_t qi = q * q;  // q^2
    for (idx_t i = 2; i <= n + 1; i++) {
        result *= (qi - 1);
        qi *= q;
    }
    
    return result;
}
