/*
 * A004009  Expansion of Eisenstein series E_4(q) (alternate convention
 *          E_2(q)); theta series of E_8 lattice.
 * https://oeis.org/A004009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; a(n) = 240 * Sum_{d|n} d^3 for n >= 1
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 251, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    
    uint64_t sigma3 = 0;
    uint64_t n64 = (uint64_t)n;
    
    for (uint64_t i = 1; i * i <= n64; i++) {
        if (n64 % i == 0) {
            sigma3 += i * i * i;
            
            if (i != n64 / i) {
                uint64_t j = n64 / i;
                sigma3 += j * j * j;
            }
        }
    }
    
    return (seq_t)(240 * sigma3);
}
