/*
 * A032614  Concatenation of n and n + 9 or {n,n+9}.
 * https://oeis.org/A032614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(n, n+9) = n * 10^digits(n+9) + (n+9)
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
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
    seq_t n_val      = (seq_t)n;
    seq_t n_plus9    = n_val + (seq_t)9;        /* n + 9 as seq_t */
    int    digits    = num_digits(n_plus9);      /* number of decimal digits of n+9 */
    seq_t  pow10d    = pow10(digits);          /* 10^digits */
    seq_t  result    = n_val * pow10d + n_plus9;
    return result;
}
