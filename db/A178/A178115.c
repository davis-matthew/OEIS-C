/*
 * A178115  a(n)=(-1)^C(n+1,2)*(F(n+1)*(1+(-1)^n)/2+F(n+2)*(1-(-1)^n)/2).
 * https://oeis.org/A178115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^C(n+1,2) * (F(n+1)*(1+(-1)^n)/2 +
 *            F(n+2)*(1-(-1)^n)/2)
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* parity of the triangular number n*(n+1)/2 determines the sign */
    idx_t tri = (n * (n + 1)) / 2;
    seq_t sign = (tri & 1) ? -1 : 1;

    /* choose the Fibonacci index:
       even n  -> F(n+1)
       odd  n  -> F(n+2)   */
    idx_t k = (n & 1ULL) ? (n + 2) : (n + 1);
    seq_t f = fib(k);

    return sign * f;
}
