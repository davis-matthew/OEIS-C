/*
 * A023552  Convolution of natural numbers >= 3 and Fibonacci numbers.
 * https://oeis.org/A023552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+4) + 2*Fibonacci(n+2) - (n+5)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 988 (2026-11-01)
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
    /* Compute Fibonacci terms n+4 and n+2 */
    seq_t fn4 = fib(n + 4);
    seq_t fn2 = fib(n + 2);

    /* a(n) = fn4 + 2*fn2 - (n+5) */
    seq_t result = fn4 + (seq_t)2 * fn2 - ((seq_t)n + 5);
    return result;
}
