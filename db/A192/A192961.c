/*
 * A192961  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Fibonacci(n+5) - (n^2 + 4*n + 10)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the utility fib(idx_t) to obtain Fibonacci numbers. */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n+5) */
    seq_t f = fib(n + 5);

    /* 2 * Fibonacci(n+5) */
    seq_t val = 2 * f;

    /* Subtract n^2 + 4*n + 10 */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t sub   = n_sq + 4 * (seq_t)n + 10;

    return val - sub;
}
