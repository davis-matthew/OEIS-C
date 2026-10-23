/*
 * A192068  a(n) = Fibonacci(2*n) - (n mod 2).
 * https://oeis.org/A192068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n) - (n mod 2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Sequence starts at offset 1; behaviour for n < 1 is irrelevant. */
    idx_t m = n << 1;            /* m = 2 * n */
    seq_t fn = fib(m);           /* Fibonacci(2*n) */
    if (n & 1)                   /* if n is odd subtract 1 */
        fn -= 1;
    return fn;
}
