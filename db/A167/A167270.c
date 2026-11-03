/*
 * A167270  a(n) = Fibonacci(n + 2) + floor(n/2).
 * https://oeis.org/A167270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n + 2) + floor(n/2)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 995 (2026-11-01)
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
    /* Compute Fibonacci(n + 2) */
    seq_t f = fib(n + 2);

    /* floor(n/2) as an unsigned integer */
    seq_t half = (seq_t)(n >> 1);

    return f + half;
}
