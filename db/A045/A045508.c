/*
 * A045508  Concatenate factorials.
 * https://oeis.org/A045508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of k! for 1 ≤ k ≤ n in decimal
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 38 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
    seq_t result = 0;
    seq_t factorial = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        factorial = factorial * (seq_t)i;
        int d = num_digits(factorial);            /* number of decimal digits of i! */
        result = result * pow10((idx_t)d) + factorial;
    }
    return result;
}
