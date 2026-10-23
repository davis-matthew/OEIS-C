/*
 * A179042  Partial sums of ceiling(Fibonacci(n)/4).
 * https://oeis.org/A179042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Fib(n+2) + 2*n + 2) / 4
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f   = fib(n + 2);          /* Fib(n+2) */
    seq_t two_n = (seq_t)n * (seq_t)2;
    seq_t numerator = f + two_n + (seq_t)2;
    return numerator / (seq_t)4;
}
