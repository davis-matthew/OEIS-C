/*
 * A134165  Let P(A) be the power set of an n-element set A. Then a(n) = the
 *          number of pairs of elements {x,y} of P(A) for which either 0) x
 *          and y are disjoint and for which either x is a subset of y or y is
 *          a subset of x, or 1) x and y are disjoint and for which x is not a
 *          subset of y and y is not a subset...
 * https://oeis.org/A134165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)(4^n - 2*3^n + 5*2^n - 2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 906 (2026-11-01)
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include utility headers for exponentiation */
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
    seq_t p2 = pow2(n);          /* 2^n   */
    seq_t p3 = pow3(n);          /* 3^n   */
    seq_t p4 = pow4(n);          /* 4^n   */

    seq_t term = p4 - (seq_t)2 * p3 + (seq_t)5 * p2 - (seq_t)2;
    return term / (seq_t)2;
}
