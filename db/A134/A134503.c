/*
 * A134503  a(n) = Fibonacci(7n + 5).
 * https://oeis.org/A134503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(7*n + 5)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the Fibonacci number at index 7*n + 5. */
    idx_t idx = (idx_t)7 * n + (idx_t)5;
    return fib(idx);
}
