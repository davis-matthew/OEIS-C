/*
 * A129728  a(n) = 2*(n-1) + Fibonacci(n).
 * https://oeis.org/A129728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1) + Fibonacci(n)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* For the OEIS offset, n >= 1. Behaviour for n == 0 is undefined. */
    seq_t fib_n = fib(n);                    /* Fibonacci(n)  (fib(0)=0, fib(1)=1) */
    seq_t term  = ((seq_t)2) * ((seq_t)(n - 1)) + fib_n;
    return term;
}
