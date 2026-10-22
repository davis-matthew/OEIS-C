/*
 * A004959  a(n) = ceiling(n*phi^4), where phi is the golden ratio, A001622.
 * https://oeis.org/A004959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n * phi^4), where phi^4 = (7 + 3*sqrt(5))/2
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    // Compute ceiling(n * (7 + 3*sqrt(5))/2) using scaled integer arithmetic
    // Scale = 2^30 for high precision without overflow
    uint64_t scale = 1ULL << 30;
    uint64_t five_scale_sq = 5 * scale * scale;
    uint64_t sqrt_5_scaled = a000037_isqrt(five_scale_sq);
    
    seq_t n_seq = (seq_t)n;
    seq_t numerator_part = (seq_t)(7 * scale) + (seq_t)(3 * sqrt_5_scaled);
    seq_t numerator = n_seq * numerator_part;
    seq_t denominator = (seq_t)(2 * scale);
    
    seq_t quotient = numerator / denominator;
    seq_t remainder = numerator % denominator;
    
    return quotient + (remainder > 0 ? 1 : 0);
}
