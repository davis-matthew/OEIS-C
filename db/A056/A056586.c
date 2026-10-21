/*
 * A056586  Ninth power of Fibonacci numbers A000045.
 * https://oeis.org/A056586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)^9 where F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    /* Compute the nth Fibonacci number */
    fib_pair(n, &fn, &fn1);

    /* Compute the ninth power of fn */
    seq_t result = fn;
    for (int i = 1; i < 9; ++i)
        result *= fn;   /* seq_t supports multiplication for all supported types */

    return result;
}
