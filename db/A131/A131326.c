/*
 * A131326  Row sums of A131325.
 * https://oeis.org/A131326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*Fibonacci(n+1)-2 if n even, a(n) = 3*Fibonacci(n+1) if
 *            n odd
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f   = fib(n + 1);   /* Fibonacci(n+1) */
    seq_t val = f * 3;       /* 3*Fibonacci(n+1) */
    if ((n & 1) == 0)        /* even n → subtract 2 */
        val -= 2;
    return val;
}
