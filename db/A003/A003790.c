/*
 * A003790  Order of universal Chevalley group A_n (7).
 * https://oeis.org/A003790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(n(n+1)/2) * ∏_{i=2}^{n+1} (7^i - 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    
    idx_t exp = n * (n + 1) / 2;
    seq_t result = a000248_pow(7, exp);
    
    for (idx_t i = 2; i <= n + 1; i++) {
        seq_t power = a000248_pow(7, i);
        result = result * (power - 1);
    }
    
    return result;
}
