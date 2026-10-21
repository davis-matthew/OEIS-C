/*
 * A081013  a(n) = Fibonacci(4*n+3) - 2, or Fibonacci(2*n)*Lucas(2*n+3).
 * https://oeis.org/A081013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n+3) - 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the index 4*n+3 in a type safe manner */
    idx_t idx = n << 2;   /* multiply by 4 */
    idx += 3;             /* add 3 */

    /* fib takes an idx_t and returns Fibonacci(idx_t) as seq_t */
    seq_t f = fib(idx);

    /* Subtract 2 (converted to seq_t) */
    return f - (seq_t)2;
}
