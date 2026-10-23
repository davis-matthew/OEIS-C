/*
 * A037157  Convolution of natural numbers n >= 1 with Fibonacci numbers F(k),
 *          for k >= 6.
 * https://oeis.org/A037157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n+10) - (47+13*n)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 37, u64 n <= 83, u128 n <= 176,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f   = fib((idx_t)(n + 10));
    seq_t sub = (seq_t)47 + (seq_t)13 * (seq_t)n;
    return f - sub;
}
