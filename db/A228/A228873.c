/*
 * A228873  a(n) = F(n) * F(n+1) * F(n+2) * F(n+3), the product of four
 *          consecutive Fibonacci numbers, A000045.
 * https://oeis.org/A228873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n) * F(n+1) * F(n+2) * F(n+3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 860 (2026-11-01)
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
    /* Get the first two Fibonacci numbers */
    seq_t fn  = fib(n);     /* F(n)   */
    seq_t fn1 = fib(n+1);   /* F(n+1) */

    /* Compute the next two Fibonacci numbers */
    seq_t fn2 = fn + fn1;   /* F(n+2) = F(n) + F(n+1) */
    seq_t fn3 = fn2 + fn1;  /* F(n+3) = F(n+2) + F(n+1) */

    /* Multiply the four numbers, using seq_t arithmetic only */
    seq_t prod = fn;
    prod *= fn1;
    prod *= fn2;
    prod *= fn3;

    return prod;
}
