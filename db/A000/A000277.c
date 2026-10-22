/*
 * A000277  3*n - 2*floor(sqrt(4*n+5)) + 5.
 * https://oeis.org/A000277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 2*floor(sqrt(4*n+5)) + 5
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt_floor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_floor_u64.h"

seq_t a(idx_t n)
{
    uint64_t s = isqrt_floor_u64((uint64_t)4 * (uint64_t)n + 5);
    seq_t v = (seq_t)n * 3 + 5;
    v -= (seq_t)s * 2;
    return v;
}
