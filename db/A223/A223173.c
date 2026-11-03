/*
 * A223173  Poly-Cauchy numbers c_3^(-n).
 * https://oeis.org/A223173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^(n+1) - 3^(n+1) + 4^n
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 286 (2026-11-01)
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t two   = pow2(n + 1);  /* 2^(n+1) */
    seq_t three = pow3(n + 1);  /* 3^(n+1) */
    seq_t four  = pow4(n);      /* 4^n      */

    return two - three + four;
}
