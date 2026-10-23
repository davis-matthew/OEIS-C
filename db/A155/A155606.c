/*
 * A155606  a(n) = 8^n + 3^n - 1.
 * https://oeis.org/A155606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n + 3^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    seq_t val8 = pow_uint(8, n);
    seq_t val3 = pow_uint(3, n);
    return val8 + val3 - (seq_t)1;
}
