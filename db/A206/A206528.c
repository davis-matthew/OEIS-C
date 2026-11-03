/*
 * A206528  5^n concatenated with itself.
 * https://oeis.org/A206528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A020338(5^n) (concatenate 5^n with itself)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 197 (2026-11-01)
 * Helpers:   ipow, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
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
    /* Compute 5^n */
    seq_t v = ipow((seq_t)5, (seq_t)n);
    /* Number of decimal digits of v */
    idx_t d = (idx_t)num_digits(v);
    /* 10^d */
    seq_t p = pow10(d);
    /* Concatenate v with itself: v * 10^d + v */
    return v * p + v;
}
