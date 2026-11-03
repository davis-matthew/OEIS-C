/*
 * A166749  Numbers that are the sum or product of two numbers, such that the
 *          sum and product have reversed digits.
 * https://oeis.org/A166749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   For n <= 8: a(n) = [0,4,18,27,49,72,81,94] respectively. For n >
 *            8: if n odd: a(n) = 5 * 10^((n+1)/2 - 3) - 1 if n even: a(n) =
 *            10^(n/2 - 2) - 6 (OEIS offset 1)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-01)
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
    if (n <= 8) {
        static const seq_t tbl[8] = {
            (seq_t)0,
            (seq_t)4,
            (seq_t)18,
            (seq_t)27,
            (seq_t)49,
            (seq_t)72,
            (seq_t)81,
            (seq_t)94
        };
        return tbl[n - 1];
    }

    if (n & 1U) {                     /* n is odd */
        idx_t exp = ((n + 1) / 2) - 3U;
        seq_t p   = pow10(exp);
        return (seq_t)5 * p - (seq_t)1;
    } else {                          /* n is even */
        idx_t exp = (n / 2) - 2U;
        seq_t p   = pow10(exp);
        return p - (seq_t)6;
    }
}
