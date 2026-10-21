/*
 * A088305  a(0) = 1, a(n) = Fibonacci(2*n). It has the property that a(n) =
 *          1*a(n-1) + 2*a(n-2) + 3*a(n-3) + 4*a(n-4) + ...
 * https://oeis.org/A088305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(2*n); a(0) = 1 (special case)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return fib(n * (idx_t)2);
}
