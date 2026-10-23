/*
 * A180674  a(n) = Fibonacci(n+9) - Fibonacci(9).
 * https://oeis.org/A180674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+9) - Fibonacci(9)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 177,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Fibonacci(0)=0, Fibonacci(1)=1 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci(9) = 34 */
    return fib(n + 9) - (seq_t)34;
}
