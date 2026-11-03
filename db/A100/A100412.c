/*
 * A100412  a(n) = 8*10^n - 7.
 * https://oeis.org/A100412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 8*10^n - 7
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 484 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

seq_t a(idx_t n)
{
    seq_t tenpow = pow10(n);
    seq_t result = tenpow * (seq_t)8;
    result -= (seq_t)7;
    return result;
}
