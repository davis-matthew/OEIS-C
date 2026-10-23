/*
 * A119703  a(n) = n^3*binomial(2*n, n)*Fibonacci(n).
 * https://oeis.org/A119703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * binomial(2*n, n) * Fibonacci(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) */
    seq_t fn = fib(n);
    if (fn == 0) {
        /* For n == 0 the result is 0 regardless of the other terms. */
        return 0;
    }

    /* Compute binomial(2*n, n) */
    seq_t bin = binomial(2 * n, n);

    /* Compute n^3 */
    seq_t nseq = (seq_t)n;
    seq_t n3 = nseq * nseq * nseq;

    /* Multiply all parts together */
    return fn * bin * n3;
}
