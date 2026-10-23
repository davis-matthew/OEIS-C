/*
 * A155612  a(n) = 7^n - 3^n + 1.
 * https://oeis.org/A155612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 3^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    seq_t pw7 = pow_uint(7, n);
    seq_t pw3 = pow_uint(3, n);
    return pw7 - pw3 + (seq_t)1;
}
