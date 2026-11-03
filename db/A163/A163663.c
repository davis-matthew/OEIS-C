/*
 * A163663  a(0) = 0 and A059153(n-1) written in base 2 otherwise.
 * https://oeis.org/A163663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n≥1, a(n) = 10^(n+1) * (10^n - 1) / 9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
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
        return (seq_t)0;
    seq_t p10n  = pow10(n);               /* 10ⁿ   */
    seq_t ones  = (p10n - (seq_t)1) / (seq_t)9;  /* (10ⁿ−1)/9  */
    seq_t p10n1 = pow10(n + 1);           /* 10ⁿ⁺¹  */
    return ones * p10n1;
}
