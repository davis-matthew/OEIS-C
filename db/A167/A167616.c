/*
 * A167616  a(n) = Fibonacci(n) - 5.
 * https://oeis.org/A167616
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) - 5, offset 5 (so a(5)=0)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t v = fib(n);
    v -= (seq_t)5;
    return v;
}
