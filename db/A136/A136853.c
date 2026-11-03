/*
 * A136853  Numbers k such that k and k^2 use only the digits 0, 1, 3, and 9.
 * https://oeis.org/A136853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 0;
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 985 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

//          a(2n)   = 10^(n‑1)
//          a(2n+1) = 3·10^(n‑1)   (n ≥ 1)
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
    if (n == 1) return (seq_t)0;

    if ((n & 1) == 0) {           // even n = 2n
        idx_t exp = (n >> 1) - 1; // n/2 - 1
        return pow10(exp);
    } else {                      // odd n = 2n+1
        idx_t exp = (n - 3) >> 1; // (n-3)/2
        seq_t base = pow10(exp);
        return base * (seq_t)3;   // multiply by 3
    }
}
