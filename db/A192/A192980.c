/*
 * A192980  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+4) + Lucas(n+3) - (n^2 + 3*n + 7) Lucas(n) =
 *            Fibonacci(n-1) + Fibonacci(n+1)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 41, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute needed Fibonacci numbers */
    seq_t f_n4 = fib(n + 4);   /* F(n+4)   */
    seq_t f_n2 = fib(n + 2);   /* F(n+2)   */

    /* Lucas(n+3) = F(n+2) + F(n+4) */
    seq_t luc_n3 = f_n2 + f_n4;

    /* Compute n^2 + 3*n + 7 */
    seq_t nseq = (seq_t)n;
    seq_t sub   = nseq * nseq + (seq_t)3 * nseq + (seq_t)7;

    /* Final result */
    return f_n4 + luc_n3 - sub;
}
