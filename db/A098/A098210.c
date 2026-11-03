/*
 * A098210  a(n) = -1 + A093137(n)^2.
 * https://oeis.org/A098210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (1/9)*(10^n - 1)*(5 + 10^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 99 (2026-11-01)
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
    seq_t pow10n = pow10(n);
    seq_t repunit = (pow10n - 1) / 9;
    seq_t t = pow10n + 5;
    return repunit * t;
}
