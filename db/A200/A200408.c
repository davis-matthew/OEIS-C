/*
 * A200408  a(n) = -4 + 5*Fibonacci(n+1)^2.
 * https://oeis.org/A200408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -4 + 5*Fibonacci(n+1)^2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f   = fib(n + 1);   /* Fibonacci(n+1) */
    seq_t sq  = f * f;        /* Fibonacci(n+1)^2 */
    seq_t res = 5 * sq;       /* 5*Fibonacci(n+1)^2 */
    res -= 4;                 /* a(n) = ... - 4 */
    return res;
}
