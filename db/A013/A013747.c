/*
 * A013747  a(n) = 10^(3*n + 2).
 * https://oeis.org/A013747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(3*n + 2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 91 (2026-11-01)
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

/* Computes the OEIS sequence A013747:
 *   a(n) = 10^(3*n + 2)
 *   with offset 0, so a(0) = 100. */
seq_t a(idx_t n)
{
    idx_t exp = n * 3 + 2;
    return pow10(exp);
}
