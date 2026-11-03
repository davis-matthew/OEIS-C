/*
 * A037140  Convolution of natural numbers n >= 1 with Fibonacci numbers F(k),
 *          for k >= 5.
 * https://oeis.org/A037140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n+9) - (29+8*n)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 177,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 965 (2026-11-01)
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
    seq_t fn  = fib(n + 9);
    seq_t sub = (seq_t)29 + ((seq_t)8 * (seq_t)n);
    return fn - sub;
}
