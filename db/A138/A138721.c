/*
 * A138721  Concatenation of n digits 1, n digits 0 and n digits 1.
 * https://oeis.org/A138721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (100^n + 1)*(10^n - 1)/9
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 298 (2026-11-01)
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
    /* 10^n and 10^(2n) */
    seq_t p10_n  = pow10(n);
    seq_t p10_2n = pow10(2 * n);

    /* (10^(2n)+1)*(10^n-1) */
    seq_t num = (p10_2n + 1) * (p10_n - 1);

    /* Exact division by 9 */
    return num / 9;
}
