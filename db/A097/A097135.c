/*
 * A097135  a(0) = 1; for n>0, a(n) = 3*Fibonacci(n).
 * https://oeis.org/A097135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n = 0) ; a(n) = 3 * Fibonacci(n) (n > 0)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function fib(idx_t n) defined in ../utils/fib.h */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t f = fib(n);
    return f * (seq_t)3;
}
