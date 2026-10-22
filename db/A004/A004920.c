/*
 * A004920  a(n) = floor(n*phi^5), where phi is the golden ratio, A001622.
 * https://oeis.org/A004920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi^5)
 * Range:     u8 n <= 23, u16 n <= 5909, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"
#include "../utils/u128_to_seq.h"

seq_t a(idx_t n) {
    static const uint64_t scale = 1000000000ULL;  // 10^9
    static uint64_t sqrt5_scaled = 0;
    
    if (sqrt5_scaled == 0) {
        sqrt5_scaled = a000037_isqrt(5ULL * scale * scale);
    }
    
    unsigned __int128 n_u128 = (unsigned __int128)n;
    unsigned __int128 numerator = n_u128 * (11ULL * scale + 5ULL * sqrt5_scaled);
    unsigned __int128 denominator = 2ULL * scale;
    unsigned __int128 result = numerator / denominator;
    
    return u128_to_seq(result);
}
