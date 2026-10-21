/*
 * A074331  a(n) = Fibonacci(n+1) - (1 + (-1)^n)/2.
 * https://oeis.org/A074331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+1) - (1+(-1)^n)/2
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n + 1);
    return (n & 1) ? f : (f - 1);
}
