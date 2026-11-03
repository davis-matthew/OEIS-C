/*
 * A083453  a(n) = (concatenation of numbers from n to 1) - n^n.
 * https://oeis.org/A083453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = (concatenation of numbers from n to 1) - n^n
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 14, i128 n <= 23,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 95 (2026-11-01)
 * Helpers:   ipow, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/ipow.h"
#include "../utils/num_digits0.h"

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
    seq_t concat = 0;
    for (idx_t k = n; k; --k) {
        uint64_t d = num_digits0(k);          /* number of decimal digits in k */
        seq_t mult = pow10((idx_t)d);         /* 10^d as seq_t */
        concat = concat * mult + (seq_t)k;    /* append k to the right */
    }
    seq_t power = ipow((seq_t)n, (seq_t)n); /* n^n */
    return concat - power;
}
