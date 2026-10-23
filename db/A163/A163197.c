/*
 * A163197  a(n) = (1/4)* L(n)^2 * F(n+1)^2 * L(n-1) * F(n+2), where F(n) and
 *          L(n) are the Fibonacci and Lucas numbers, respectively.
 * https://oeis.org/A163197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (F(2n+1)^3 - 3*F(2n+1) - 2*(-1)^n) / 4
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 15, i128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

/* Computes the n-th term of A163197 */
seq_t a(idx_t n) {
    /* compute index for the Fibonacci number: 2*n+1 */
    idx_t m = (n << 1) + 1;

    /* F = F(2n+1) */
    seq_t f = fib(m);

    /* f^2 and f^3 */
    seq_t f_sq  = f * f;
    seq_t f_cub = f_sq * f;

    /* 3*F(2n+1) */
    seq_t three_f = f * 3;

    /* (-1)^n */
    seq_t sign = (n & 1) ? (seq_t)(-1) : (seq_t)1;

    /* -2*(-1)^n */
    seq_t two_sign = sign * 2;

    /* numerator: f^3 - 3f - 2*(-1)^n */
    seq_t numer = f_cub - three_f - two_sign;

    /* divide by 4 (exact division) */
    return numer / 4;
}
