/*
 * A168576  a(n) = (10^n+1)^4.
 * https://oeis.org/A168576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n + 1)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 100 (2026-11-01)
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
    seq_t t = pow10(n);
    t = t + 1;
    seq_t t2 = t * t;
    seq_t t4 = t2 * t2;
    return t4;
}
