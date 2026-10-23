/*
 * A037213  Expansion of Sum_{n>=0} n*q^(n^2).
 * https://oeis.org/A037213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sqrt(n) if n is a perfect square, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   local_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/local_isqrt_u64.h"

seq_t a(idx_t n) {
    idx_t r = local_isqrt_u64(n);
    if (r * r == n) return (seq_t)r;
    return (seq_t)0;
}
