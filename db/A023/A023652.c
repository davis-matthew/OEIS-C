/*
 * A023652  Convolution of (F(2), F(3), F(4), ...) and odd numbers.
 * https://oeis.org/A023652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+6) - 4*n - 8
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 995 (2026-11-01)
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
    seq_t fn = fib(n + 6);
    seq_t sub = (seq_t)n * 4 + 8;
    return fn - sub;
}
