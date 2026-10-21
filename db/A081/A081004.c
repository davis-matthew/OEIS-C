/*
 * A081004  a(n) = Fibonacci(4n+2) + 1, or Fibonacci(2n+2)*Lucas(2n).
 * https://oeis.org/A081004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n+2) + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* index = 4*n + 2 fits in idx_t for the intended range */
    idx_t idx = 4 * n + 2;
    seq_t value = fib(idx);   /* Fibonacci(index) */
    ++value;                  /* + 1 */
    return value;
}
