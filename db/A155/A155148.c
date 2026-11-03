/*
 * A155148  Numbers k such that k^4 has exactly 2 different decimal digits.
 * https://oeis.org/A155148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2 a(2) = 3 a(n) = 10^(n-2) for n >= 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
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
    if (n == 1) return 2;
    if (n == 2) return 3;
    /* n >= 3 */
    idx_t e = n - 2;          /* exponent for 10^e */
    return pow10(e);          /* returns 10^e as a seq_t */
}
