/*
 * A192963  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*Fibonacci(n+3) + 4*Fibonacci(n+2) - (n^2 + 5*n + 10)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f2 = fib(n + 2);
    seq_t f3 = fib(n + 3);
    seq_t term = 3 * f3 + 4 * f2;

    seq_t sn = (seq_t)n;
    seq_t sub = sn * sn + 5 * sn + 10;

    return term - sub;
}
