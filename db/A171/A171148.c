/*
 * A171148  a(n) = 3 + 10^n + 3*100^n.
 * https://oeis.org/A171148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3 + 10^n + 3*100^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 444 (2026-11-01)
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
    /* 100^n = (10^2)^n = 10^(2n) */
    idx_t powexp = n << 1;
    seq_t p10   = pow10(n);
    seq_t p100  = pow10(powexp);
    return 3 + p10 + 3 * p100;
}
