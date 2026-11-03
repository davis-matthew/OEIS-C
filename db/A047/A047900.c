/*
 * A047900  a(1) = 8; for n > 1, a(n+1) = a(n) * sum of digits of a(n).
 * https://oeis.org/A047900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 233 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047900: a(1) = 8; for n > 1, a(n) = 64 * 10^(n-2) */
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
    if (n == 1)
        return (seq_t)8;
    /* for n >= 2, the formula holds */
    return (seq_t)64 * pow10(n - 2);
}
