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
 * Validated: standard, pass, 47 terms through n = 185 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    seq_t f = fib(n + 1);
    seq_t sq = f * f;
    if ((n & 1) == 0)
        return sq - 1;
    else
        return sq;
}
