/*
 * A198973  a(n) = 8*10^n - 1.
 * https://oeis.org/A198973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*10^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 175 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    seq_t ten_pow = pow10(n);
    return ten_pow * (seq_t)8 - (seq_t)1;
}
