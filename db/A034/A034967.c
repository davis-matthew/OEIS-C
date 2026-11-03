/*
 * A034967  Sum of digits of numbers between 0 and (10^n)-1.
 * https://oeis.org/A034967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 45*n*10^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 961 (2026-11-01)
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
    if (n == 0)
        return 0;

    seq_t coeff = (seq_t)n * 45;
    seq_t p10   = pow10(n - 1);
    return coeff * p10;
}
