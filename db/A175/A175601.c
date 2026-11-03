/*
 * A175601  a(n) = 7*(10^n - 9).
 * https://oeis.org/A175601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(10^n - 9)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 96 (2026-11-01)
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
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       (offset behaviour is irrelevant). */
    if (n == 0)
        return (seq_t)0;

    /* Compute 10^n, subtract 9, and multiply by 7. */
    seq_t val = pow10(n);
    val -= (seq_t)9;
    val *= (seq_t)7;
    return val;
}
