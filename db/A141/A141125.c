/*
 * A141125  Hankel transform of a transform of Fibonacci numbers.
 * https://oeis.org/A141125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^n*(cos(Pi*n/2)+2*sin(Pi*n/2))
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 32 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    seq_t res = pow2(n);            /* 2^n */
    switch (n & 3) {                /* n modulo 4 determines sign and factor */
        case 0:                     /* 2^n * 1  */
            break;
        case 1:                     /* 2^n * 2  */
            res = seq_mul(res, (seq_t)2);
            break;
        case 2:                     /* 2^n * -1 */
            res = seq_mul(res, (seq_t)-1);
            break;
        case 3:                     /* 2^n * -2 */
            res = seq_mul(res, (seq_t)-2);
            break;
    }
    return res;
}
