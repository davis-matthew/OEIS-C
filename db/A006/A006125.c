/*
 * A006125  a(n) = 2^(n*(n-1)/2).
 * https://oeis.org/A006125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 2^(n*(n-1)/2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"

seq_t a(idx_t n) {
    idx_t exponent = n * (n - 1) / 2;
    return pw2(exponent);
}
