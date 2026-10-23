/*
 * A163196  a(n) = L(n)^2 * F(n+1)^2 * L(n-1) * F(n+2), where F(n) and L(n)
 *          are the Fibonacci and Lucas numbers, respectively.
 * https://oeis.org/A163196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = F(2n+1)^3 - 3*F(2n+1) - 2*(-1)^n
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 15, i128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Compute the n-th term of OEIS sequence A163196.
   Uses the closed form a(n) = F(2n+1)^3 - 3*F(2n+1) - 2*(-1)^n.
   The helper function fib(idx_t) (from utils/fib.h) returns the
   Fibonacci number F(idx_t).  Since this expression is an exact
   integer for every n, it works for any seq_t that can hold the
   result.  For seq_t values that cannot represent the result,
   overflow will occur as in the reference implementation. */
seq_t a(idx_t n)
{
    /* (-1)^n: 1 if n even, -1 if n odd */
    seq_t sign = (n & 1) ? (seq_t)(-1) : (seq_t)1;

    /* calculate F(2n+1) */
    idx_t k = (idx_t)(2 * n + 1);
    seq_t f = fib(k);

    /* f^3 - 3*f - 2*sign */
    seq_t f2 = f * f;
    seq_t f3 = f2 * f;

    seq_t result = f3 - (seq_t)3 * f - (seq_t)2 * sign;
    return result;
}
