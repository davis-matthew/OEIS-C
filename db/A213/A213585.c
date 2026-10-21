/*
 * A213585  Principal diagonal of the convolution array A213584.
 * https://oeis.org/A213585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+4) + n*Fibonacci(n+3) - (4*n + 3)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 37, u64 n <= 81, u128 n <= 172,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* uses the supplied fib(idx_t) utility */

/* Include the Fibonacci helper */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0, we return 0
       (the value is irrelevant for the OEIS offset). */
    if (n == 0)
        return (seq_t)0;

    /* Compute the required Fibonacci numbers */
    seq_t fn3 = fib(n + 3);
    seq_t fn4 = fib(n + 4);

    /* Combine according to the formula */
    seq_t term1 = fn4;
    seq_t term2 = (seq_t)n * fn3;
    seq_t term3 = (seq_t)(4 * n + 3);

    return term1 + term2 - term3;
}
