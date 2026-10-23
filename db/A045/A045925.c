/*
 * A045925  a(n) = n*Fibonacci(n).
 * https://oeis.org/A045925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*Fibonacci(n)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n);               /* F(n) */
    return (seq_t)n * f;            /* a(n) = n*F(n) */
}
