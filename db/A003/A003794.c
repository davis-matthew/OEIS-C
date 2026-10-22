/*
 * A003794  Order of simple Chevalley group A_n (4).
 * https://oeis.org/A003794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^{n(n+1)/2} * Product_{i=2}^{n+1} (4^i - 1) / gcd(n+1,
 *            3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

static idx_t gcd_u64(idx_t a, idx_t b) {
    while (b) {
        idx_t tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

seq_t a(idx_t n) {
    idx_t exponent = (n * (n + 1)) / 2;
    seq_t result = a000272_ipow((seq_t)4, exponent);
    
    seq_t power_of_4 = (seq_t)16;
    for (idx_t i = 2; i <= n + 1; i++) {
        result *= (power_of_4 - 1);
        power_of_4 *= (seq_t)4;
    }
    
    idx_t g = gcd_u64(n + 1, 3);
    result /= (seq_t)g;
    
    return result;
}
