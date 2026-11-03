/*
 * A004530  Expansion of (theta_2(0, x) + theta_3(0, x) + theta_4(0, x)) / 2
 *          in powers of x^(1/4).
 * https://oeis.org/A004530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2 if n > 0 is square and divisible by 16; 1 if n = 0 or n
 *            is odd square; 0 otherwise
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 138 terms through n = 9728 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    uint64_t sqrt_n = isqrt_u64((uint64_t)n);
    
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
