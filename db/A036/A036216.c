/*
 * A036216  Expansion of 1/(1 - 3*x)^4; 4-fold convolution of A000244 (powers
 *          of 3).
 * https://oeis.org/A036216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n * binomial(n+3, 3)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 70,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 371 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n) {
    seq_t tri = (seq_t)(n + 1);
    tri *= (n + 2);
    tri *= (n + 3);
    tri /= 6;
    seq_t pow3 = ipow_i32((seq_t)3, (int)n);
    return tri * pow3;
}
