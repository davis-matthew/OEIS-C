/*
 * A027956  a(n) = T(2n+1, n+4), T given by A027948.
 * https://oeis.org/A027956
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2*n+10) - (34650 + 33360*n + 16065*n^2 +
 *            5089*n^3 + 1260*n^4 + 280*n^5 + 16*n^7)/630 for n >= 4; a(3) = 1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 15, u64 n <= 41, u128 n <= 88,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 3)                     /* a(3) = 1 */
        return (seq_t)1;
    if (n < 3)                      /* undefined region, return 0 */
        return (seq_t)0;

    /* cast index to seq_t for arithmetic */
    seq_t N1 = (seq_t)n;
    seq_t N2 = N1 * N1;            /* n^2 */
    seq_t N3 = N2 * N1;            /* n^3 */
    seq_t N4 = N3 * N1;            /* n^4 */
    seq_t N5 = N4 * N1;            /* n^5 */
    seq_t N7 = N5 * N2;            /* n^7 = n^5 * n^2 */

    /* polynomial part */
    seq_t poly = (seq_t)34650 +
                 (seq_t)33360 * N1 +
                 (seq_t)16065 * N2 +
                 (seq_t)5089 * N3 +
                 (seq_t)1260 * N4 +
                 (seq_t)280   * N5 +
                 (seq_t)16    * N7;

    seq_t poly_div = poly / (seq_t)630;

    /* Fibonacci part */
    seq_t fib_val = fib((idx_t)(2 * n + 10));

    return fib_val - poly_div;
}
