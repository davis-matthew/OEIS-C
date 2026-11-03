/*
 * A171583  a(n) = 10^n*(4+3*10^n)+3.
 * https://oeis.org/A171583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n*(4+3*10^n)+3
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

seq_t a(idx_t n) {
    seq_t p = pow10(n);       /* p = 10^n */
    seq_t tmp = p * 3;        /* tmp = 3*10^n */
    tmp += 4;                 /* tmp = 4 + 3*10^n */
    seq_t res = p * tmp;      /* res = 10^n * (4+3*10^n) */
    res += 3;                 /* res = 10^n*(4+3*10^n)+3 */
    return res;
}
