/*
 * A020522  a(n) = 4^n - 2^n.
 * https://oeis.org/A020522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 2^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 163 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t four_pow = pow4(n);
    seq_t two_pow  = pow2(n);
    return four_pow - two_pow;
}
