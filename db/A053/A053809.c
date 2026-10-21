/*
 * A053809  Second partial sums of A001891.
 * https://oeis.org/A053809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n+10) - (2*n^3 + 27*n^2 + 145*n + 324)/6
 * Range:     u8 none, u16 n <= 14, u32 n <= 37, u64 n <= 83, u128 n <= 176,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t poly = 2*ns*ns*ns + 27*ns*ns + 145*ns + 324;
    seq_t sub = poly / 6;
    seq_t fib_val = fib(n + 10);
    return fib_val - sub;
}
