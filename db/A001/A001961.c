/*
 * A001961  A Beatty sequence: floor(n * (sqrt(5) - 1)).
 * https://oeis.org/A001961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (sqrt(5) - 1))
 * Range:     u8 none, u16 none, u32 n <= 3528, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
    uint64_t scale = 1ULL << 20;
    uint64_t sqrt5_scaled = a000037_isqrt(5ULL * scale * scale);
    uint64_t alpha_scaled = sqrt5_scaled - scale;
    
    seq_t result = ((seq_t)n * (seq_t)alpha_scaled) / (seq_t)scale;
    
    return result;
}
