/*
 * A004968  a(n) = ceiling(n*phi^13), where phi is the golden ratio, A001622.
 * https://oeis.org/A004968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^13), where phi is the golden ratio
 * Range:     u8 n <= 0, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    static long double phi_13 = 0.0L;
    
    if (phi_13 == 0.0L) {
        long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;
        phi_13 = powl(phi, 13.0L);
    }
    
    return (seq_t)(long long)ceill((long double)n * phi_13);
}
