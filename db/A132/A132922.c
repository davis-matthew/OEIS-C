/*
 * A132922  Row sums of triangle A132921.
 * https://oeis.org/A132922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1) + Fibonacci(n+2) - 1
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 438 (2026-11-01)
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
    if (n == 0) return (seq_t)0;
    seq_t tri = (seq_t)n * ((seq_t)n - 1);
    seq_t f   = fib(n + 2);
    return tri + f - 1;
}
