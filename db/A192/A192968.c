/*
 * A192968  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*Fib(n+2) - (n^2+3*n+6)/2
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the Fibonacci function from the utilities: fib(idx_t n). */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the Fibonacci term Fib(n+2). */
    seq_t f = fib(n + 2);

    /* 3 * Fib(n+2). */
    seq_t res = f + f + f;   /* efficient 3× multiplication */

    /* Compute (n^2 + 3n + 6) / 2 as an integer. */
    seq_t nn = (seq_t)n;
    seq_t sub = nn * (nn + 3); /* n^2 + 3n   */
    sub += 6;                 /* +6         */
    sub /= 2;                 /* divide by 2 */

    /* Final result. */
    res -= sub;
    return res;
}
