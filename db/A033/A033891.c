/*
 * A033891  a(n) = Fibonacci(4*n+3).
 * https://oeis.org/A033891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n+3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* compute the Fibonacci index: 4*n + 3 */
    idx_t m = (idx_t)4 * n + (idx_t)3;
    return fib(m);
}
