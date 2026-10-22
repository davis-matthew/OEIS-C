/*
 * A004530  Expansion of (theta_2(0, x) + theta_3(0, x) + theta_4(0, x)) / 2
 *          in powers of x^(1/4).
 * https://oeis.org/A004530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2 if n > 0 is square and divisible by 16; 1 if n = 0 or n
 *            is odd square; 0 otherwise
 * Range:     u8 n <= 9728, u16 n <= 9728, u32 n <= 9728, u64 n <= 9728,
 *            u128 n <= 9728, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    uint64_t sqrt_n = a000037_isqrt((uint64_t)n);
    
    // Check if n is a perfect square
    if (sqrt_n * sqrt_n != n) {
        return 0;
    }
    
    // n is a perfect square
    if (sqrt_n & 1) {
        return 1;
    }
    
    // sqrt_n is even
    if ((sqrt_n & 3) == 0) {
        return 2;
    }
    
    return 0;
}
