/*
 * A177097  a(n) = 6*(10^n-5).
 * https://oeis.org/A177097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(10^n-5)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(1)=30, a(2)=570, a(n)=11*a(n-1)-10*a(n-2) for n≥1. */
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
    if (n == 0)          // offset starts at 1; undefined for n==0
        return 0;
    seq_t ten_pow = pow10(n);    // 10^n
    seq_t val = ten_pow - (seq_t)5; // 10^n - 5
    val *= (seq_t)6;              // 6*(10^n - 5)
    return val;
}
