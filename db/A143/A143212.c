/*
 * A143212  a(n) = Fibonacci(n) * (Fibonacci(n+2) - 1).
 * https://oeis.org/A143212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) * (Fibonacci(n+2) - 1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour for n < 1 is unspecified */
    seq_t fn  = fib(n);            /* Fibonacci(n)      */
    seq_t fn2 = fib(n + 2);        /* Fibonacci(n+2)    */
    return fn * (fn2 - 1);         /* product as required */
}
