/*
 * A192952  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*Fibonacci(n+2) - (3*n + 4)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* compute Fibonacci(n+2) */
    seq_t f = fib((idx_t)(n + 2));

    /* 4 * Fibonacci(n+2) */
    seq_t res = f * (seq_t)4;

    /* subtract (3*n + 4) */
    seq_t sub = ((seq_t)3 * (seq_t)n) + (seq_t)4;
    res -= sub;

    return res;
}
