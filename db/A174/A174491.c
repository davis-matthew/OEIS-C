/*
 * A174491  Denominator in the coefficient of x^n in exp( Sum_{m>=1}
 *          x^m/(m*2^(m^2)) ).
 * https://oeis.org/A174491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = 2^(n^2) * highest power of 2 dividing n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;                 /* a(0) = 1 */

    /* exponent of the power of two factor: 2^(n^2) */
    idx_t exp = n * n;                   /* n^2 fits in idx_t for all intended inputs */

    seq_t pow = pow2(exp);                /* 2^(n^2) as seq_t */

    /* highest power of two dividing n: n & -n */
    seq_t low = (seq_t)(n & -(idx_t)n);   /* works for unsigned n */

    return seq_mul(pow, low);            /* a(n) = pow2(n^2) * (n & -n) */
}
