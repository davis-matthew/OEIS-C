/*
 * A001446  a(n) = (4^n + 4^[n/2])/2.
 * https://oeis.org/A001446
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n + 4^floor(n/2))/2 = 2^(2n-1) + 2^(2*floor(n/2)-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   a001446_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001446_pow2.h"

seq_t a(idx_t n) {
    idx_t h = n / 2;
    seq_t term1 = a001446_pow2(2 * n - 1);
    seq_t term2 = a001446_pow2(2 * h - 1);
    return (seq_t)(term1 + term2);
}
