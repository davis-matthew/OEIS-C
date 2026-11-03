/*
 * A141265  Concatenate first n factorials in reverse order.
 * https://oeis.org/A141265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat_{k=1}^{n+1} k! (reverse order, largest first)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    seq_t res  = (seq_t)1;   /* a(0) = 1! */
    seq_t fact = (seq_t)1;   /* factorial of (0+1) = 1! */

    for (idx_t i = 0; i < n; ++i) {
        /* factorial of (i+2)! = previous fact * (i+2) */
        fact = fact * (seq_t)(i + 2);

        /* prepend new factorial to the current result */
        idx_t shift = (idx_t)num_digits(res);            /* decimal digits of previous result */
        res = fact * pow10(shift) + res;
    }
    return res;
}
