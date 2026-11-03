/*
 * A000161  Number of partitions of n into 2 squares.
 * https://oeis.org/A000161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = card { {a,b} : a^2+b^2 = n, 0 <= a <= b } (direct search
 *            over a with 2*a^2 <= n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 124 terms through n = 9908 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    uint64_t i;
    for (i = 0; i * i <= n - i * i && i * i <= n; i++) {
        uint64_t r = n - i * i;
        uint64_t b = isqrt_u64(r);
        if (b * b == r) count++;
    }
    return count;
}
