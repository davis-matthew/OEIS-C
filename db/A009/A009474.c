/*
 * A009474  a(n) is the concatenation of n and 9n.
 * https://oeis.org/A009474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = n * 10^{digit_len(9*n)} + 9*n
 * Range:     u8 n <= 2, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 944 (2026-11-01)
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
    seq_t nine_n   = (seq_t)n * 9;              /* 9 * n */
    idx_t  digits  = (idx_t)num_digits(nine_n);  /* number of decimal digits in 9*n */
    seq_t  pow     = pow10(digits);            /* 10^digits   */
    return (seq_t)n * pow + nine_n;            /* concatenation */
}
