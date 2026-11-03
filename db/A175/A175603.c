/*
 * A175603  a(n) = 8*(10^n-5).
 * https://oeis.org/A175603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 8*(10^n-5)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 96 (2026-11-01)
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
    /* Compute 10^n using the utility function. */
    seq_t p10 = pow10(n);

    /* a(n) = 8*(10^n-5) = 8*(p10-5) */
    seq_t res = p10 - (seq_t)5;
    res *= (seq_t)8;
    return res;
}
