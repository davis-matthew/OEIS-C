/*
 * A138473  a(n) = Fibonacci(8*n).
 * https://oeis.org/A138473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*a(n-1) - a(n-2), a(0)=0, a(1)=21 which equals
 *            Fibonacci(8*n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 499 (2026-11-01)
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
    if (n == 0) return 0;          /* a(0) = 0 */
    return fib(n * 8);             /* a(n) = Fibonacci(8*n) */
}
