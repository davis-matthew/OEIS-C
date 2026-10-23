/*
 * A134499  a(n) = Fibonacci(7*n+1).
 * https://oeis.org/A134499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(7*n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the Fibonacci index 7*n+1 using seq_t arithmetic,
       then call fib() (which expects idx_t). */
    seq_t m = (seq_t)7 * n + 1;          /* 7*n+1 in seq_t */
    return fib((idx_t)m);               /* Fibonacci number */
}
