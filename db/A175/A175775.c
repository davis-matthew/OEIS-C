/*
 * A175775  a(n) = 7*(10^n-3).
 * https://oeis.org/A175775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*(10^n-3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
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
    /* a(n) is defined only for n >= 1;  the formula works for any n */
    if (n == 0)
        return 0;           /* value below offset is irrelevant */

    seq_t p = pow10(n);          /* 10^n */
    seq_t r = p - (seq_t)3;      /* 10^n - 3 */
    return (seq_t)7 * r;         /* 7 * (10^n - 3) */
}
