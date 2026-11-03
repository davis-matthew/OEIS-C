/*
 * A171736  a(n) = 10^n*(2+7*10^n)+7.
 * https://oeis.org/A171736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n*(2+7*10^n)+7
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 14,
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
    seq_t t = pow10(n);      /* t = 10^n */
    seq_t res = t * t;      /* res = 10^(2n) */
    res *= 7;               /* res = 7*10^(2n) */
    res += t * 2;           /* res += 2*10^n */
    res += 7;               /* res += 7 */
    return res;              /* a(n) = 10^n*(2+7*10^n)+7 */
}
