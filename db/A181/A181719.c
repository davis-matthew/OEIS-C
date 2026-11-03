/*
 * A181719  a(n) = A133473(n+1)^2.
 * https://oeis.org/A181719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5 + 10^n)^2 / 9 (OEIS A181719)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 97 (2026-11-01)
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
    /* compute 10^n */
    seq_t pow10n = pow10(n);
    /* (5 + 10^n) is always divisible by 3, so divide first */
    seq_t half = (pow10n + (seq_t)5) / (seq_t)3;
    /* final result: ((5+10^n)/3)^2 */
    return half * half;
}
