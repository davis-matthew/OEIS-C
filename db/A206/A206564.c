/*
 * A206564  Fibonacci sequence beginning 14, 13.
 * https://oeis.org/A206564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14 * Fibonacci(n) - Fibonacci(n-1)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly for safety */
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)13;

    /* Compute Fibonacci numbers using the provided utility */
    seq_t fn  = fib(n);     /* Fib(n)   */
    seq_t fn1 = fib(n-1);   /* Fib(n-1) */

    /* a(n) = 14*Fib(n) - Fib(n-1) */
    return ((seq_t)14 * fn) - fn1;
}
