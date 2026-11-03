/*
 * A000934  Chromatic number (or Heawood number) Chi(n) of surface of genus n.
 * https://oeis.org/A000934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = floor((7 + sqrt(1 + 48*n))/2)
 * Range:     u8 n <= 5312, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9564 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    uint64_t v = (uint64_t)1 + (uint64_t)48 * (uint64_t)n;
    uint64_t s = isqrt_u64(v);
    return (seq_t)((7 + s) / 2);
}
