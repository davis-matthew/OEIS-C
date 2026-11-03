/*
 * A110807  n times largest n-digit number.
 * https://oeis.org/A110807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n*(10^n - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 928 (2026-11-01)
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
    seq_t ten_pow = pow10(n);      /* 10^n */
    seq_t res = ten_pow - (seq_t)1;
    res *= (seq_t)n;
    return res;
}
