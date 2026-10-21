/*
 * A223851  Poly-Cauchy numbers c_4^(-n).
 * https://oeis.org/A223851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^n + 11*3^n - 3*2^(2*n+1) - 3*2^(n+1)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Compute the powers with a fast exponentiation routine.  */
    seq_t p5   = pow_int(5, n);         /* 5^n            */
    seq_t p3   = pow_int(3, n);         /* 3^n            */
    seq_t p22n = pow_int(2, 2*n + 1);   /* 2^(2n+1)       */
    seq_t p2n  = pow_int(2, n + 1);     /* 2^(n+1)        */

    /* Combine the terms according to the closed‑form formula.  */
    return p5 + 11 * p3 - 3 * p22n - 3 * p2n;
}
