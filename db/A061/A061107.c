/*
 * A061107  a(0) = 0, a(1) = 1, a(n) is the concatenation of a(n-2) and a(n-1)
 *          for n > 1.
 * https://oeis.org/A061107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = concatenate(a(n-1), a(n-2)) in
 *            base‑10 digit form; thus a(2) = 10, a(3) = 101, a(4) = 10110,
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

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
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_prev2 = 0;   /* a(0) */
    seq_t a_prev1 = 1;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* number of decimal digits of a_prev2; treat 0 as having 1 digit "0" */
        idx_t shift = (a_prev2 == 0) ? 1 : (idx_t)num_digits(a_prev2);
        cur = a_prev1 * pow10(shift) + a_prev2;   /* concatenate */
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return a_prev1;
}
