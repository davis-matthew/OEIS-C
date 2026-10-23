/*
 * A180671  a(n) = Fibonacci(n+6) - Fibonacci(6).
 * https://oeis.org/A180671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+6) - Fibonacci(6) (where Fibonacci(6) = 8)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    return fib(n + 6) - (seq_t)8;
}
