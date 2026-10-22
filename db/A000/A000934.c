/*
 * A000934  Chromatic number (or Heawood number) Chi(n) of surface of genus n.
 * https://oeis.org/A000934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = floor((7 + sqrt(1 + 48*n))/2)
 * Range:     u8 n <= 5312, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a934_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a934_isqrt.h"

seq_t a(idx_t n) {
    uint64_t v = (uint64_t)1 + (uint64_t)48 * (uint64_t)n;
    uint64_t s = a934_isqrt(v);
    return (seq_t)((7 + s) / 2);
}
