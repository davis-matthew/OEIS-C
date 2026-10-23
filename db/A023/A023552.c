/*
 * A023552  Convolution of natural numbers >= 3 and Fibonacci numbers.
 * https://oeis.org/A023552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+4) + 2*Fibonacci(n+2) - (n+5)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci terms n+4 and n+2 */
    seq_t fn4 = fib(n + 4);
    seq_t fn2 = fib(n + 2);

    /* a(n) = fn4 + 2*fn2 - (n+5) */
    seq_t result = fn4 + (seq_t)2 * fn2 - ((seq_t)n + 5);
    return result;
}
