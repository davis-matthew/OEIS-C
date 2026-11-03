/*
 * A004201  Accept one, reject one, accept two, reject two, ...
 * https://oeis.org/A004201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n + t(t+1)/2, where t = floor((-1+sqrt(8*n-7))/2)
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9376 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    uint64_t sqrt_val = isqrt_u64(8 * n - 7);
    uint64_t t = (sqrt_val - 1) / 2;
    return (seq_t)n + ((seq_t)t * ((seq_t)t + 1)) / 2;
}
