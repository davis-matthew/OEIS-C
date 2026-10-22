/*
 * A000267  Integer part of square root of 4n+1.
 * https://oeis.org/A000267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, tabf
 * Formula:   a(n) = floor(sqrt(4n+1)); with r = floor(sqrt(n)), a(n) = 2r if
 *            n < r^2 + r, else 2r + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a267_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a267_isqrt.h"

seq_t a(idx_t n)
{
    uint64_t r = a267_isqrt((uint64_t)n);
    seq_t res = (seq_t)r;
    res = res * 2;
    if ((uint64_t)n >= r * r + r) res += 1;
    return res;
}
