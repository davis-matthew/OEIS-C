/*
 * A139114  Concatenation of n-th Fibonacci number and n.
 * https://oeis.org/A139114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(Fibonacci(n), n)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 171,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 985 (2026-11-01)
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
    /* Fibonacci number F(n) */
    seq_t fn = fib(n);

    /* number of decimal digits of n */
    int d = 0;
    idx_t t = n;
    do { d++; t /= 10; } while (t);

    /* 10^d as a seq_t */
    seq_t pow = 1;
    for (int i = 0; i < d; i++)
        pow *= 10;

    /* concatenate: F(n) * 10^d + n */
    return fn * pow + (seq_t)n;
}
