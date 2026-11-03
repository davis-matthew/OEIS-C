/*
 * A002938  The minimal sequence from solving n^3 - m^2 = a(n).
 * https://oeis.org/A002938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(n^3 - floor(sqrt(n^3))^2, (floor(sqrt(n^3))+1)^2 -
 *            n^3), the distance from n^3 to the nearest square
 * Range:     u8 n <= 47, u16 n <= 1647, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 7910 (2026-11-01)
 * Helpers:   isqrt_seq, isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n) {
    if (n < ((idx_t)1 << 42)) {
        unsigned __int128 c = (unsigned __int128)n * n * n;
        unsigned __int128 r = isqrt_u128(c);
        unsigned __int128 lo = c - r * r;
        unsigned __int128 hi = (r + 1) * (r + 1) - c;
        uint64_t d = (uint64_t)(lo <= hi ? lo : hi);
        return (seq_t)d;
    } else {
        seq_t v = (seq_t)n;
        seq_t c = v * v * v;
        seq_t r = isqrt_seq(c);
        seq_t lo = c - r * r;
        seq_t hi = (r + 1) * (r + 1) - c;
        return lo <= hi ? lo : hi;
    }
}
