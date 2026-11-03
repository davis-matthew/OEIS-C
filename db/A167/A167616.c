/*
 * A167616  a(n) = Fibonacci(n) - 5.
 * https://oeis.org/A167616
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) - 5, offset 5 (so a(5)=0)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 793 (2026-11-01)
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
    seq_t v = fib(n);
    v -= (seq_t)5;
    return v;
}
