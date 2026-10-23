/*
 * A192975  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+3) + 3*Lucas(n+2) - 2*(2*n+5)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    seq_t fn3, fn4, fn_np1, fn_np2;

    /* Get Fibonacci(n+3) and Fibonacci(n+4) */
    fib_pair(n + 3, &fn3, &fn4);

    /* Get Fibonacci(n+1) and Fibonacci(n+2) */
    fib_pair(n + 1, &fn_np1, &fn_np2);

    /* Lucas(n+2) = Fibonacci(n+1) + Fibonacci(n+3) */
    seq_t lucas_np2 = fn_np1 + fn3;

    /* Assemble the expression */
    seq_t term1 = fn3;                            /* Fibonacci(n+3) */
    seq_t term2 = lucas_np2 * ((seq_t)3);         /* 3*Lucas(n+2) */
    seq_t term3 = ((seq_t)2) * ((seq_t)(2 * (idx_t)n + 5)); /* 2*(2*n+5) */

    return term1 + term2 - term3;
}
