/*
 * A213580  Principal diagonal of the convolution array A213579.
 * https://oeis.org/A213580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+3) + n*Fibonacci(n+2) - 2*(n+1)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 38, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n == 0 we return 0,
       which is not used for the OEIS offset. */
    if (n == 0)
        return 0;

    seq_t fn2 = fib(n + 2);          /* Fibonacci(n+2) */
    seq_t fn3 = fib(n + 3);          /* Fibonacci(n+3) */

    seq_t term = fn3 + (seq_t)n * fn2;
    seq_t sub  = 2 * ((seq_t)(n + 1));

    return term - sub;
}
