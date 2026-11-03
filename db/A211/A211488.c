/*
 * A211488  a(n) = Fibonacci(n^2) - Fibonacci(n).
 * https://oeis.org/A211488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n^2) - Fibonacci(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 68 (2026-11-01)
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
    idx_t idx_sq = n * n;         /* n^2, guaranteed to fit in idx_t for n up to 10000 */
    seq_t fn   = fib(n);          /* Fibonacci(n)   */
    seq_t fn_sq = fib(idx_sq);    /* Fibonacci(n^2) */

    return fn_sq - fn;            /* result fits in seq_t */
}
