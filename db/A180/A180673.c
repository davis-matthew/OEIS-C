/*
 * A180673  a(n) = Fibonacci(n+8) - Fibonacci(8).
 * https://oeis.org/A180673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+8) - Fibonacci(8)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 275 (2026-11-01)
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
    seq_t f = fib(n + 8);
    return f - (seq_t)21;
}
