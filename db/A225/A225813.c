/*
 * A225813  a(n) = (10^n)^2 + 7*(10^n) + 1.
 * https://oeis.org/A225813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n)^2 + 7*(10^n) + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 490 (2026-11-01)
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
    seq_t p  = pow10(n);          /* 10^n */
    seq_t p2 = p * p;             /* (10^n)^2 */
    return p2 + 7 * p + 1;        /* (10^n)^2 + 7*(10^n) + 1 */
}
