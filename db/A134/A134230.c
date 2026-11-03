/*
 * A134230  a(n) = (10^n+1)^2 - 1.
 * https://oeis.org/A134230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n + 1)^2 - 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 461 (2026-11-01)
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
    seq_t p = pow10(n);           /* 10^n */
    seq_t t = p + (seq_t)1;       /* 10^n + 1 */
    seq_t res = t * t;            /* (10^n + 1)^2 */
    res -= (seq_t)1;              /* - 1 */
    return res;
}
