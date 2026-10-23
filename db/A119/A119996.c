/*
 * A119996  Numerator of Sum_{k=1..n} 1/(Fibonacci(k)*Fibonacci(k+2)).
 * https://oeis.org/A119996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(n) = Fibonacci(n+1)*Fibonacci(n+2) - 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f1 = fib(n + 1);   /* Fibonacci(n+1) */
    seq_t f2 = fib(n + 2);   /* Fibonacci(n+2) */
    return f1 * f2 - (seq_t)1;
}
