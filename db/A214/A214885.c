/*
 * A214885  a(n) = Sum_{k=0..n} (-1)^k*F(k)*F(k+3), where F=A000045 (Fibonacci
 *          numbers).
 * https://oeis.org/A214885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((-1)^n * Fibonacci(2*n + 4) - 4*n - 3)/5
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 21, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci(2*n + 4) */
    idx_t idx = 2 * n + 4;
    seq_t f = fib(idx);

    /* Multiply by (-1)^n */
    if (n & 1)  /* odd n -> negative sign */
        f = -f;

    /* Compute 4*n + 3 */
    seq_t term = (seq_t)4 * (seq_t)n + 3;

    /* Numerator: (-1)^n * F(2*n+4) - (4*n + 3) */
    seq_t num = f - term;

    /* Divide by 5 (exact division) */
    return num / 5;
}
