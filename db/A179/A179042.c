/*
 * A179042  Partial sums of ceiling(Fibonacci(n)/4).
 * https://oeis.org/A179042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Fib(n+2) + 2*n + 2) / 4
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-01)
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
    seq_t f   = fib(n + 2);          /* Fib(n+2) */
    seq_t two_n = (seq_t)n * (seq_t)2;
    seq_t numerator = f + two_n + (seq_t)2;
    return numerator / (seq_t)4;
}
