/*
 * A004202  Skip 1, take 1, skip 2, take 2, skip 3, take 3, etc.
 * https://oeis.org/A004202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = n + A000217(A002024(n))
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n) {
        seq_t n_seq = (seq_t)n;
    
    // Compute m = A002024(n) = floor((sqrt(8n - 1) + 1) / 2)
    uint64_t u = 8 * (uint64_t)n - 1;
    uint64_t t = a000037_isqrt(u);
    seq_t m = (seq_t)((t + 1) / 2);
    
    // Compute A000217(m) = m * (m + 1) / 2, avoiding overflow
    seq_t tri;
    if (m & 1) {
        tri = m * ((m + 1) / 2);
    } else {
        tri = (m / 2) * (m + 1);
    }
    
    return n_seq + tri;
}
