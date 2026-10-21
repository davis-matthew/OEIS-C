/*
 * A052952  a(n) = Fibonacci(n+2) - (1-(-1)^n)/2.
 * https://oeis.org/A052952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+2) - (1-(-1)^n)/2
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t fn2 = fib(n + 2);            /* Fibonacci number F(n+2) */
    if (n & 1ULL)                      /* n odd → subtract 1 */
        return fn2 - 1;
    else                               /* n even → no subtraction */
        return fn2;
}
