/*
 * A192951  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+4) + 2*Fibonacci(n+2) - (3*n + 5)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 382 (2026-11-01)
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
    seq_t fn4 = fib(n + 4);
    seq_t fn2 = fib(n + 2);
    seq_t sub = (seq_t)n * 3 + 5;
    return fn4 + fn2 * 2 - sub;
}
