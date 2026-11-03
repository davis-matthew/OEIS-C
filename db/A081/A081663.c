/*
 * A081663  a(n) = Fibonacci(2n+1) + n*2^(n-1).
 * https://oeis.org/A081663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(2n+1) + n*2^(n-1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
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
    /* 1. Fibonacci term */
    seq_t fib_term = fib(2 * n + 1);

    /* 2. Power of two: 2^(n-1) for n > 0, otherwise 1 (unused as multiplier will be 0) */
    seq_t pow2 = 1;
    for (idx_t i = 1; i < n; ++i) {
        pow2 *= 2;
    }

    /* 3. n * 2^(n-1) and sum with the Fibonacci term */
    seq_t mult = (seq_t)n * pow2;
    return fib_term + mult;
}
