/*
 * A166536  A product of consecutive doubled Fibonacci numbers.
 * https://oeis.org/A166536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+1)*F(n+2) + (1 - (-1)^n)/2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (F = Fibonacci sequence, F(0)=0, F(1)=1) */

#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n+1) and Fibonacci(n+2) */
    seq_t f1 = fib(n + 1);
    seq_t f2 = fib(n + 2);

    /* Product of the two Fibonacci numbers */
    seq_t result = f1 * f2;

    /* Add 1 if n is odd (equivalent to (1 - (-1)^n)/2) */
    if (n & 1U)
        result += (seq_t)1;

    return result;
}
