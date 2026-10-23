/*
 * A027944  a(n) = T(2n+1, n+4), T given by A027935.
 * https://oeis.org/A027944
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2*n+9) - (8*n^6 + 12*n^5 + 110*n^4 + 465*n^3 +
 *            1412*n^2 + 2943*n + 3060)/90 (offset 3)
 * Range:     u8 none, u16 n <= 3, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* undefined for n<3 */

    /* Use the fib function from the utilities to get Fibonacci(2*n+9) */
    idx_t m = 2 * n + 9;
    seq_t fib_m = fib(m);

    /* Compute the polynomial part P(n)/90 */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;

    seq_t poly = (seq_t)8 * n6
               + (seq_t)12 * n5
               + (seq_t)110 * n4
               + (seq_t)465 * n3
               + (seq_t)1412 * n2
               + (seq_t)2943 * nn
               + (seq_t)3060;

    seq_t poly_div = poly / (seq_t)90;

    return fib_m - poly_div;
}
