/*
 * A027941  a(n) = Fibonacci(2*n + 1) - 1.
 * https://oeis.org/A027941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Fibonacci(2*n + 1) - 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* compute the index of the Fibonacci number: 2*n + 1 */
    idx_t k = n + n + 1;      /* safe for all n in the range requested */
    seq_t f = fib(k);         /* fib(k) returns Fibonacci(k) in seq_t */
    f -= 1;                   /* a(n) = Fib(2*n + 1) - 1 */
    return f;
}
