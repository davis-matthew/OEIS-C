/*
 * A178664  a(n) = 2^n concatenated with itself.
 * https://oeis.org/A178664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2^n concatenated with 2^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 196 (2026-11-01)
 * Helpers:   num_digits, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/pow2.h"

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
    /* Compute 2^n */
    seq_t v = pow2(n);

    /* Number of decimal digits in 2^n */
    int d = num_digits(v);

    /* 10^d */
    seq_t pow10_d = pow10((idx_t)d);

    /* Concatenation: v * 10^d + v */
    return v * pow10_d + v;
}
