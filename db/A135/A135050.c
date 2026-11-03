/*
 * A135050  Numbers k such that sum of digits of k^3 is 8. Multiples of 10 are
 *          omitted.
 * https://oeis.org/A135050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2,5,8 for n = 1..3; a(n) = 10^(n-3) + 1 for n ≥ 4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
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
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)8;
    return pow10(n - 3) + (seq_t)1;
}
