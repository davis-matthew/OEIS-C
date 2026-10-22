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
 * Helpers:   a1030_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1030_isqrt.h"

seq_t a(idx_t n) {
    unsigned __int128 hi = 2 * (unsigned __int128)n + 1;
    unsigned __int128 lo = 2 * (unsigned __int128)n - 1;
    unsigned __int128 r1 = a1030_isqrt(hi * hi / 2);
    unsigned __int128 r0 = a1030_isqrt(lo * lo / 2);
    return (seq_t)(unsigned)(r1 - r0);
}
