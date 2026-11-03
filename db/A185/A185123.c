/*
 * A185123  a(n) = n 9's sandwiched between two 1's.
 * https://oeis.org/A185123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less, base
 * Formula:   a(n) = 20*10^n - 9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
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
    seq_t v = pow10(n);        /* v = 10^n */
    v = v * (seq_t)20;         /* v = 20*10^n */
    v = v - (seq_t)9;          /* v = 20*10^n - 9 */
    return v;
}
