/*
 * A014162  Apply partial sum operator thrice to Fibonacci numbers.
 * https://oeis.org/A014162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+6) - (n^2 + 7*n + 16)/2
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t fib_val = fib(n + 6);
    seq_t t = n_seq * n_seq + 7 * n_seq + (seq_t)16;
    t /= 2;
    return fib_val - t;
}
