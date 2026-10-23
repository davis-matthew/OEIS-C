/*
 * A122678  Invariant number of polygons of n-circum-C_5 H_5 systems.
 * https://oeis.org/A122678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*Fibonacci(2*n-1)-4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t f = fib(2 * n - 1);
    return f * (seq_t)5 - (seq_t)4;
}
