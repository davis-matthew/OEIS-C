/*
 * A019551  a(n) is the concatenation of n and 3n.
 * https://oeis.org/A019551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = n concatenated with 3n (decimal)
 * Range:     u8 n <= 3, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 951 (2026-11-01)
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
    seq_t n_seq = (seq_t)n;          /* convert index to seq_t */
    seq_t t     = n_seq * 3;         /* compute 3n */
    int    td   = num_digits(t);      /* number of decimal digits of 3n */
    seq_t exp10 = pow10((idx_t)td); /* 10^td */
    return n_seq * exp10 + t;        /* concatenate n and 3n */
}
