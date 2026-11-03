/*
 * A014162  Apply partial sum operator thrice to Fibonacci numbers.
 * https://oeis.org/A014162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+6) - (n^2 + 7*n + 16)/2
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 889 (2026-11-01)
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
    seq_t n_seq = (seq_t)n;
    seq_t fib_val = fib(n + 6);
    seq_t t = n_seq * n_seq + 7 * n_seq + (seq_t)16;
    t /= 2;
    return fib_val - t;
}
