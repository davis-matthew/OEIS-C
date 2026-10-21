/*
 * A236191  a(n) = (-1)^floor( (n-1) / 3 ) * F(n), where F = Fibonacci.
 * https://oeis.org/A236191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^floor((n-1)/3) * F(n) where F(n) is the Fibonacci
 *            number (F(0)=0,F(1)=1)
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 46, i64 n <= 92, i128 n <= 184,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* F(0) = 0, sign irrelevant */
    seq_t fn = fib(n);                    /* compute F(n) */
    /* sign = (-1)^k with k = floor((n-1)/3) */
    idx_t k = (n - 1) / 3;                /* safe: n >= 1 */
    if (k & 1)                            /* k odd → negative sign */
        return -fn;
    return fn;                            /* positive sign */
}
