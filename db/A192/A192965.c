/*
 * A192965  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+4) + 3*Fibonacci(n+2) - (n^2 + 3*n + 6)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 977 (2026-11-01)
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

seq_t a(idx_t n) {
    seq_t fn4 = fib(n + 4);
    seq_t fn2 = fib(n + 2);
    seq_t quad = (seq_t)n * (seq_t)n + (seq_t)3 * (seq_t)n + (seq_t)6;
    seq_t res = fn4 + (seq_t)3 * fn2 - quad;
    return res;
}
