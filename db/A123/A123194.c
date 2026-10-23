/*
 * A123194  a(n) = (n+1)*Fibonacci(n+2) + 3.
 * https://oeis.org/A123194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*Fibonacci(n+2) + 3
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 38, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n + 2);
    return ((seq_t)n + 1) * f + 3;
}
