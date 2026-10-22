/*
 * A007607  Skip 1, take 2, skip 3, etc.
 * https://oeis.org/A007607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = floor(sqrt(n) + 1/2)^2 + n
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
        
    uint64_t k = a000037_isqrt(n);
    uint64_t m = (n > k * (k + 1)) ? k + 1 : k;
    
    return (seq_t)m * (seq_t)m + (seq_t)n;
}
