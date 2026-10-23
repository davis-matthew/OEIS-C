/*
 * A194157  Product of first n nonzero even-indexed Fibonacci numbers F(2),
 *          F(4), F(6), ..., F(2*n).
 * https://oeis.org/A194157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..n} F(2*i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        prod *= fib(2 * i);
    }
    return prod;
}
