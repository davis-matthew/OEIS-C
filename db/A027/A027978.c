/*
 * A027978  a(n) = self-convolution of row n of array T given by A027960.
 * https://oeis.org/A027978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A027978
   Formula: a(n) = 2*(n+1)*Lucas(2*n) + Fibonacci(2*n-4)   (n ≥ 2)
   Initial values: a(0)=1, a(1)=11
   (Lucas number L_k is computed as Fib(k-1)+Fib(k+1) ready from the fast
   doubling Fibonacci implementation in the utils.) */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* handle initial values explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)42;
    if (n == 3) return (seq_t)145;
    if (n == 4) return (seq_t)473;

    /* for n ≥ 2 */
    /* compute needed Fibonacci values */
    seq_t f2n_minus4 = fib(2*n - 4);   /* Fibonacci(2*n-4) */
    seq_t f2n_minus1 = fib(2*n - 1);   /* Fibonacci(2*n-1) */
    seq_t f2n_plus1  = fib(2*n + 1);   /* Fibonacci(2*n+1) */

    /* Lucas number L_2n = Fib(2n-1)+Fib(2n+1) */
    seq_t lucas_2n = f2n_minus1 + f2n_plus1;

    /* compute a(n) = 2*(n+1)*Lucas(2n) + Fibonacci(2n-4) */
    seq_t n_plus1 = (seq_t)(n + 1);          /* cast index to seq_t */
    seq_t result = (seq_t)2 * n_plus1 * lucas_2n + f2n_minus4;

    return result;
}
