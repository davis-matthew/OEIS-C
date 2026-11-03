/*
 * A147997  Number of nonnegative even integers <= Fibonacci(n).
 * https://oeis.org/A147997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + floor(Fibonacci(n)/2)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-01)
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

/*  Computes a(n) = number of nonnegative even integers <= Fibonacci(n)  */
seq_t a(idx_t n)
{
    seq_t f = fib(n);        /* Fibonacci(n)  */
    /* For non‑negative f, f/2 is floor(f/2) in C integer division */
    seq_t res = f / 2 + 1;
    return res;
}
