/*
 * A138473  a(n) = Fibonacci(8*n).
 * https://oeis.org/A138473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*a(n-1) - a(n-2), a(0)=0, a(1)=21 which equals
 *            Fibonacci(8*n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */
    return fib(n * 8);             /* a(n) = Fibonacci(8*n) */
}
