/*
 * A131412  a(n) = n*(Fibonacci(n) - 1) + Fibonacci(n + 2) - 1.
 * https://oeis.org/A131412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(Fibonacci(n) - 1) + Fibonacci(n + 2) - 1
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 39, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci of n and n+2 */
    seq_t fn  = fib(n);       /* Fib(n)   */
    seq_t fn2 = fib(n + 2);   /* Fib(n+2) */

    /* a(n) = n*(Fib(n)-1) + Fib(n+2) - 1 */
    return (seq_t)n * (fn - 1) + fn2 - 1;
}
