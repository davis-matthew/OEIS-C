/*
 * A007412  The noncubes: a(n) = n + floor((n + floor(n^(1/3)))^(1/3)).
 * https://oeis.org/A007412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = n + floor((n + floor(n^(1/3)))^(1/3))
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   icbrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/icbrt_u64.h"

seq_t a(idx_t n) {
    uint64_t c1 = icbrt_u64((uint64_t)n);
    uint64_t c2 = icbrt_u64((uint64_t)n + c1);
    return (seq_t)n + (seq_t)c2;
}
