/*
 * A171630  a(n) = 10^n*(5+3*10^n)+3.
 * https://oeis.org/A171630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n*(5+3*10^n)+3
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
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
    seq_t p10 = pow10(n);           /* 10^n */
    seq_t p10sq = p10 * p10;        /* 10^(2n) */
    seq_t term = (seq_t)5 * p10 + (seq_t)3 * p10sq + (seq_t)3;
    return term;
}
