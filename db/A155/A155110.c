/*
 * A155110  a(n) = 8*Fibonacci(2n+1).
 * https://oeis.org/A155110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 8*Fib(2*n+1)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute 2*n + 1 as an idx_t */
    idx_t k = (n << 1) + 1;          /* 2*n + 1 */

    /* Fibonacci(k) returns a seq_t value */
    seq_t f = fib(k);

    /* Scale by 8 – multiplication works for all seq_t sizes */
    return f * (seq_t)8;
}
