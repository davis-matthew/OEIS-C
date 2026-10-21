/*
 * A064831  Partial sums of A001654, or sum of the areas of the first n
 *          Fibonacci rectangles.
 * https://oeis.org/A064831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+1)^2 if n is odd; a(n) = F(n+1)^2 - 1 if n is even
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n + 1);
    seq_t sq = f * f;
    if ((n & 1) == 0)
        return sq - 1;
    else
        return sq;
}
