/*
 * A200675  Powers of 2 repeated 4 times.
 * https://oeis.org/A200675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^floor(n/4)
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 127, u64 n <= 255,
 *            u128 n <= 511, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    return pow2(n >> 2);
}
