/*
 * A009440  a(n) is the concatenation of n and 6n.
 * https://oeis.org/A009440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = concat(n, 6*n)
 * Range:     u8 n <= 2, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 925 (2026-11-01)
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
    seq_t nseq = (seq_t)n;          /* convert index to seq_t */
    seq_t m = nseq * (seq_t)6;      /* 6 * n */
    int d = num_digits(m);           /* number of decimal digits of 6n */
    seq_t p10 = pow10((idx_t)d);    /* 10^d */
    return nseq * p10 + m;          /* concatenate: n * 10^d + 6n */
}
