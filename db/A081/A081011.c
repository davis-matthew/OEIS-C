/*
 * A081011  a(n) = Fibonacci(4n+3) + 2, or Fibonacci(2n+3)*Lucas(2n).
 * https://oeis.org/A081011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4n+3) + 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci number F(4n+3) using the provided utility */
    seq_t f = fib((idx_t)(4 * n + 3));
    /* Add 2 and return the result */
    return f + (seq_t)2;
}
