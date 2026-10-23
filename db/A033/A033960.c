/*
 * A033960  Convolution of natural numbers n >= 1 with Fibonacci numbers F(k),
 *          k >= 4.
 * https://oeis.org/A033960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n+8) - (18+5*n)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f    = fib(n + 8);          /* Fib(n+8) */
    seq_t sub  = (seq_t)18 + (seq_t)5 * (seq_t)n;
    return f - sub;
}
