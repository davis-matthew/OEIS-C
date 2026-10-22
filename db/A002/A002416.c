/*
 * A002416  a(n) = 2^(n^2).
 * https://oeis.org/A002416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   a001309_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001309_pow2.h"

seq_t a(idx_t n) {
    idx_t exp = n * n;
    return a001309_pow2(exp);
}
