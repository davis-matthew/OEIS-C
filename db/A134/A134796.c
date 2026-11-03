/*
 * A134796  Concatenation of next n positive oblong numbers.
 * https://oeis.org/A134796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenate of the next n oblong numbers. Let m =
 *            (n-1)*n/2. Then a(n) = concat_{k=m+1}^{m+n} k*(k+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-01)
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
    if (n == 0)
        return 0;               /* offset is 1; 0 is unused */

    seq_t result = 0;
    idx_t m = (n - 1) * n / 2;   /* sum of 1..(n-1) */
    idx_t start = m + 1;         /* first k in this block */

    for (idx_t k = start; k < start + n; ++k) {
        seq_t oblong = (seq_t)k * ((seq_t)k + 1);   /* k(k+1) in seq_t */
        int digits = num_digits(oblong);            /* decimal length of oblong */
        seq_t tenpow = pow10((idx_t)digits);      /* 10^digits */
        result = result * tenpow + oblong;        /* append oblong */
    }

    return result;
}
