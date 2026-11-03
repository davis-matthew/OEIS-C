/*
 * A059988  a(n) = (10^n - 1)^2.
 * https://oeis.org/A059988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n - 1)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 200 (2026-11-01)
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
    /* Compute 10^n as a seq_t, then subtract 1 and square the result. */
    seq_t t = pow10(n);   /* t = 10^n  */
    t -= 1;               /* t = 10^n - 1  */
    return t * t;         /* a(n) = (10^n - 1)^2  */
}
