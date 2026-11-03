/*
 * A013792  a(n) = 10^(4*n + 1).
 * https://oeis.org/A013792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 95 (2026-11-01)
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
    /* compute exponent 4*n + 1, safe because idx_t is unsigned 64-bit */
    idx_t exp = 4 * n + 1;
    return pow10(exp);
}
