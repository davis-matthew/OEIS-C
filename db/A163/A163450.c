/*
 * A163450  A028403 written in base 2.
 * https://oeis.org/A163450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1) = 100; for n >= 2, a(n) = 10^(2*n-1) + 10^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 220 (2026-11-01)
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
    if (n == 1) {
        return (seq_t)100;
    }
    /* n >= 2 */
    seq_t ten2n_1 = pow10((idx_t)(2 * n - 1));
    seq_t ten_n   = pow10(n);
    return ten2n_1 + ten_n;
}
