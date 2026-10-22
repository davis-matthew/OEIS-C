/*
 * A000703  Chromatic number (or Heawood number) of nonorientable surface with
 *          n crosscaps.
 * https://oeis.org/A000703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor((7+sqrt(1+24*n))/2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a703_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a703_isqrt.h"

seq_t a(idx_t n) {
    uint64_t d = (uint64_t)1 + (uint64_t)24 * (uint64_t)n;
    uint64_t s = a703_isqrt(d);
    uint64_t v = (7 + s) / 2;
    return (seq_t)v;
}
