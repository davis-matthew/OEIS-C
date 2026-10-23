/*
 * A027955  a(n) = T(2n+1, n+3), T given by A027948.
 * https://oeis.org/A027955
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2*n+8) - (630 +607*n +295*n^2 +90*n^3 +20*n^4
 *            +8*n^5)/30 for n>=3, a(2) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 42, u128 n <= 89,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)1;          /* a(2) is defined explicitly */

    /* n ≥ 3  */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;

    seq_t poly = (seq_t)630
               + (seq_t)607 * nn
               + (seq_t)295 * n2
               + (seq_t)90  * n3
               + (seq_t)20  * n4
               + (seq_t)8   * n5;

    seq_t poly_div = poly / (seq_t)30;

    idx_t fib_idx = 2 * n + 8;
    seq_t fib_val = fib(fib_idx);

    return fib_val - poly_div;
}
