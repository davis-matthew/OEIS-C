/*
 * A007606  Take 1, skip 2, take 3, etc.
 * https://oeis.org/A007606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, nice, easy
 * Formula:   a(n) = n + m*(m+1) where m = floor(sqrt(n-1))
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9857 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    uint64_t mm = isqrt_u64((uint64_t)(n - 1));
    seq_t m = (seq_t)mm;
    return (seq_t)n + m * (m + 1);
}
