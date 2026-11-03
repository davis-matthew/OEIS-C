/*
 * A001030  Fixed under 1 -> 21, 2 -> 211.
 * https://oeis.org/A001030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = -1 + floor(n*(1+sqrt(2))+1/sqrt(2)) -
 *            floor((n-1)*(1+sqrt(2))+1/sqrt(2)), computed exactly as a(n) =
 *            isqrt(floor((2n+1)^2/2)) - isqrt(floor((2n-1)^2/2)), since
 *            floor(n*(1+sqrt(2)) + 1/sqrt(2)) = n + floor((2n+1)/sqrt(2))
 * Range:     u8 n <= 8119, u16 n <= 8119, u32 n <= 8119, u64 n <= 8119,
 *            u128 n <= 8119, big any n
 * Validated: standard, pass, 121 terms through n = 7490 (2026-11-01)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n) {
    unsigned __int128 hi = 2 * (unsigned __int128)n + 1;
    unsigned __int128 lo = 2 * (unsigned __int128)n - 1;
    unsigned __int128 r1 = isqrt_u128(hi * hi / 2);
    unsigned __int128 r0 = isqrt_u128(lo * lo / 2);
    return (seq_t)(unsigned)(r1 - r0);
}
