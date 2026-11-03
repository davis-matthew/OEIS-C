/*
 * A103456  a(n) = 0^n + 10^n - 1.
 * https://oeis.org/A103456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0^n + 10^n - 1 (a(0)=1, otherwise a(n)=10^n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 409 (2026-11-01)
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
    if (n == 0)
        return (seq_t)1;
    seq_t ten_pow = pow10(n);
    return ten_pow - (seq_t)1;
}
