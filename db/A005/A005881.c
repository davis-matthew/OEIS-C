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
 * Validated: standard, pass, 121 terms through n = 9460 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int jacobi_symbol(int64_t a, uint64_t n) {
    if (n == 1) return 1;
    
    a = a % (int64_t)n;
    if (a < 0) a += (int64_t)n;
    
    int result = 1;
    
    while (a != 0) {
        while (a % 2 == 0) {
            a /= 2;
            if (n % 8 == 3 || n % 8 == 5) {
                result = -result;
            }
        }
        
        uint64_t temp = n;
        n = (uint64_t)a;
        a = (int64_t)temp;
        
        if ((a % 4 == 3) && (n % 4 == 3)) {
            result = -result;
        }
        
        a = a % (int64_t)n;
    }
    
    return (n == 1) ? result : 0;
}

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
