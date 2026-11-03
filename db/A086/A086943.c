/*
 * A086943  Integers k such that R(k+7) = 3.
 * https://oeis.org/A086943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*10^n - 7
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 291 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t v = pow10(n);      /* 10^n */
    v = v * (seq_t)3;        /* 3*10^n */
    v = v - (seq_t)7;        /* 3*10^n - 7 */
    return v;
}
