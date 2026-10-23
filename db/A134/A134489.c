/*
 * A134489  a(n) = Fibonacci(5*n + 2).
 * https://oeis.org/A134489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(5*n + 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* compute the index 5*n + 2 in a type wider than idx_t to avoid overflow */
    idx_t m = (idx_t)(((seq_t)n * (seq_t)5) + (seq_t)2);
    return fib(m);
}
