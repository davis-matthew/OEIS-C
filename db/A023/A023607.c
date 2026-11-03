/*
 * A023607  a(n) = n * Fibonacci(n+1).
 * https://oeis.org/A023607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*Fib(n+1)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 83, u128 n <= 174,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 984 (2026-11-01)
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
    /* Compute Fibonacci(n+1) */
    seq_t fn = fib(n + 1);

    /* Multiply by n, casting to seq_t first */
    return (seq_t)n * fn;
}
