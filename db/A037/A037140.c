/*
 * A037140  Convolution of natural numbers n >= 1 with Fibonacci numbers F(k),
 *          for k >= 5.
 * https://oeis.org/A037140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n+9) - (29+8*n)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 177,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t fn  = fib(n + 9);
    seq_t sub = (seq_t)29 + ((seq_t)8 * (seq_t)n);
    return fn - sub;
}
