/*
 * A125521  a(n) is the minimal difference between two distinct n-digit
 *          numbers with property that when one of them is typed into a
 *          calculator and rotated 180 degrees, the other one is seen.
 * https://oeis.org/A125521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1)=3, a(2)=6, a(n)=10*a(n-2) for n>2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the closed‑form a(n) = (n odd?3:6) * 10^((n-1)/2)  */
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
    if (n == 1) return 3;
    if (n == 2) return 6;
    /* exponent for 10 */
    idx_t e = (n - 1) / 2;
    seq_t pow = pow10(e);
    seq_t base = (n & 1) ? 3 : 6;
    return base * pow;
}
