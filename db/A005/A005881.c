/*
 * A005881  Theta series of planar hexagonal lattice (A2) with respect to
 *          edge.
 * https://oeis.org/A005881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Sum_{d | 2*n+1} JacobiSymbol(-3,d)
 * Range:     u8 n <= 1581, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n) {
    seq_t m = (seq_t)n * 2 + 1;
    seq_t sum = 0;
    
    for (seq_t d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            sum += jacobi_symbol(-3, (uint64_t)d);
            if (d != m / d) {
                sum += jacobi_symbol(-3, (uint64_t)(m / d));
            }
        }
    }
    
    return 2 * sum;
}
