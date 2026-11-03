/*
 * A049661  a(n) = (Fibonacci(6*n+1) - 1)/4.
 * https://oeis.org/A049661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Fibonacci(6*n+1) - 1)/4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 742 (2026-11-01)
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
    /* Compute index 6*n+1 (fits in idx_t for typical n) */
    idx_t k = 6*n + 1;

    /* Calculate Fibonacci(k) */
    seq_t f = fib(k);

    /* (F - 1) is always divisible by 4, giving integer result */
    return (f - 1) / 4;
}
