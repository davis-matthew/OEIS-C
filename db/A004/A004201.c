/*
 * A004201  Accept one, reject one, accept two, reject two, ...
 * https://oeis.org/A004201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n + t(t+1)/2, where t = floor((-1+sqrt(8*n-7))/2)
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"


seq_t a(idx_t n) {
    uint64_t sqrt_val = a000037_isqrt(8 * n - 7);
    uint64_t t = (sqrt_val - 1) / 2;
    return (seq_t)n + ((seq_t)t * ((seq_t)t + 1)) / 2;
}
