/*
 * A001952  A Beatty sequence: a(n) = floor(n*(2 + sqrt(2))).
 * https://oeis.org/A001952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(n*(2 + sqrt(2))) = 2*n + floor(sqrt(2*n^2))
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9851 (2026-11-01)
 * Helpers:   isqrt_u128_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128_to_u64.h"

seq_t a(idx_t n)
{
    unsigned __int128 m = (unsigned __int128)n;
    uint64_t r = isqrt_u128_to_u64(2 * m * m);
    seq_t v = (seq_t)n;
    v += (seq_t)n;
    v += (seq_t)r;
    return v;
}
