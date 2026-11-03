/*
 * A045925  a(n) = n*Fibonacci(n).
 * https://oeis.org/A045925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*Fibonacci(n)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 939 (2026-11-01)
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
    seq_t f = fib(n);               /* F(n) */
    return (seq_t)n * f;            /* a(n) = n*F(n) */
}
