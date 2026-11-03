/*
 * A131412  a(n) = n*(Fibonacci(n) - 1) + Fibonacci(n + 2) - 1.
 * https://oeis.org/A131412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(Fibonacci(n) - 1) + Fibonacci(n + 2) - 1
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 39, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 976 (2026-11-01)
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
    /* Fibonacci of n and n+2 */
    seq_t fn  = fib(n);       /* Fib(n)   */
    seq_t fn2 = fib(n + 2);   /* Fib(n+2) */

    /* a(n) = n*(Fib(n)-1) + Fib(n+2) - 1 */
    return (seq_t)n * (fn - 1) + fn2 - 1;
}
