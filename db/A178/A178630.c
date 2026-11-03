/*
 * A178630  a(n) = 18*((10^n - 1)/9)^2.
 * https://oeis.org/A178630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*((10^n - 1)/9)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 200 (2026-11-01)
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
    /* Compute 10^n */
    seq_t v = pow10(n);
    v -= 1;          /* (10^n - 1) */
    v /= 9;          /* integer division: (10^n - 1) / 9 */
    v = v * v;       /* square */
    v *= 18;         /* multiply by 18 */
    return v;
}
