/*
 * A192956  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+3) + 4*Fibonacci(n+1) - (2*n+5)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    /* compute the two Fibonacci terms */
    seq_t f1 = fib(n + 3);        /* Fibonacci(n+3) */
    seq_t f2 = fib(n + 1);        /* Fibonacci(n+1) */

    /* combine them according to the closed‑form */
    seq_t result = f1 + (f2 * 4);

    /* subtract the linear part */
    seq_t linear = ((seq_t)n * 2) + 5;
    return result - linear;
}
