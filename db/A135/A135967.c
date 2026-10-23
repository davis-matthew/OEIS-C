/*
 * A135967  Row sums of triangle A135966.
 * https://oeis.org/A135967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} fibonacci(k*(n-k) + 1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t m = k * (n - k) + 1;
        sum += fib(m);
    }
    return sum;
}
