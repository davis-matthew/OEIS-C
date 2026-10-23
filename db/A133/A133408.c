/*
 * A133408  Numbers k such that k is a substring of both its square and its
 *          cube in base 2 (written in base 10).
 * https://oeis.org/A133408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 0 if n=1; 41 if n=8; 2^(n-2) if 2≤n<8; 2^(n-3) if n>8
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 8) return (seq_t)41;
    if (n < 8)  return pow2(n - 2);
    return pow2(n - 3);
}
