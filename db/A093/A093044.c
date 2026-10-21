/*
 * A093044  A Jacobsthal Fibonacci product: a(n) = (2^n +
 *          2*(-1)^n)*Fibonacci(n-1)/3.
 * https://oeis.org/A093044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n + 2*(-1)^n)*Fibonacci(n-1)/3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) is defined separately (fib(-1) is not defined) */
    if (n == 0) return (seq_t)1;

    /* 2^n */
    seq_t pow2n = pow2(n);

    /* Fibonacci(n-1) – fib(0) = 0 works for n = 1 */
    seq_t fnm1 = fib(n - 1);

    /* (2^n + 2*(-1)^n) : add 2 if n even, subtract 2 if n odd */
    seq_t term;
    if ((n & 1) == 0)      /* n even : (-1)^n = 1  */
        term = pow2n + (seq_t)2;
    else                   /* n odd  : (-1)^n = -1 */
        term = pow2n - (seq_t)2;

    /* multiply by Fibonacci(n-1) and divide by 3 */
    return (term * fnm1) / (seq_t)3;
}
