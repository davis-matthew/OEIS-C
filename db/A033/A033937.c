/*
 * A033937  Convolution of natural numbers n >= 1 with Fibonacci numbers F(k),
 *          k >= 3.
 * https://oeis.org/A033937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n+7) - (11 + 3*n)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the (n+7)th Fibonacci number */
    seq_t f = fib(n + 7);

    /* Compute the offset 11 + 3*n in seq_t arithmetic */
    seq_t offset = (seq_t)11 + ((seq_t)3 * (seq_t)n);

    /* Return the result */
    return f - offset;
}
