/*
 * A156561  Floor(Fibonacci(2n+1)/9).
 * https://oeis.org/A156561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(Fibonacci(2n+1)/9)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the (2n+1)th Fibonacci number and divide by 9. */
    idx_t m = (idx_t)(2 * n) + 1;
    seq_t f = fib(m);
    return f / (seq_t)9;
}
