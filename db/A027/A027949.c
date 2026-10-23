/*
 * A027949  a(n) = T(2n,n+1), T given by A027948.
 * https://oeis.org/A027949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+4) - (2*n^2 + 3*n + 3) for n>1; a(1) = 1.
 *            Fibonacci is taken from the provided utils (fib.h)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Compute Fibonacci(2*n+4) using the utility function. */
    idx_t k = 2 * n + 4;
    seq_t fib_val = fib(k);

    /* Compute the polynomial part: 2*n^2 + 3*n + 3. */
    seq_t nn = (seq_t)n;
    seq_t poly = (seq_t)2 * nn * nn + (seq_t)3 * nn + (seq_t)3;

    return fib_val - poly;
}
