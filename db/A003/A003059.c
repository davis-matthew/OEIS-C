/*
 * A003059  k appears 2k-1 times. Also, square root of n, rounded up.
 * https://oeis.org/A003059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(sqrt(n)) = floor(sqrt(n-1)) + 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 112 terms through n = 967 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n)
{
    uint64_t r = isqrt_u64((uint64_t)(n - 1));
    seq_t result = (seq_t)r;
    result += 1;
    return result;
}
