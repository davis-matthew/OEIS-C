/*
 * A169622  a(n) = a(n-1) + Fibonacci(n), a(1)=5.
 * https://oeis.org/A169622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + Fib(n+2)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return fib(n + 2) + 3;
}
