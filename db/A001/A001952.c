/*
 * A001952  A Beatty sequence: a(n) = floor(n*(2 + sqrt(2))).
 * https://oeis.org/A001952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(n*(2 + sqrt(2))) = 2*n + floor(sqrt(2*n^2))
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001952_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001952_isqrt128.h"

seq_t a(idx_t n)
{
    unsigned __int128 m = (unsigned __int128)n;
    uint64_t r = a001952_isqrt128(2 * m * m);
    seq_t v = (seq_t)n;
    v += (seq_t)n;
    v += (seq_t)r;
    return v;
}
