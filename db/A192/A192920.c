/*
 * A192920  Coefficient of x in the reduction by (x^2 -> x+1) of the
 *          polynomial F(n+4)*x^n, where F = A000045 (Fibonacci sequence).
 * https://oeis.org/A192920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+2)^2 – (–1)^n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n + 2);   /* Fibonacci(n+2) */
    seq_t sq = f * f;       /* square it */

    /* (–1)^n = 1 when n is even, –1 when n is odd */
    if ((n & 1) == 0)
        return sq - 1;      /* even n: subtract 1 */
    else
        return sq + 1;      /* odd n: add 1 */
}
