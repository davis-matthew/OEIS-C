/*
 * A081010  a(n) = Fibonacci(4n+1) + 2, or Fibonacci(2n-1)*Lucas(2n+2).
 * https://oeis.org/A081010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(4*n+1) + 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    idx_t k = 4 * n + 1;   /* 4*n+1 fits in idx_t for all expected n */
    seq_t val = fib(k);    /* compute Fibonacci(k) */
    val += (seq_t)2;       /* add 2 */
    return val;
}
