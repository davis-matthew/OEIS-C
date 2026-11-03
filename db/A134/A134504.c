/*
 * A134504  a(n) = Fibonacci(7n + 6).
 * https://oeis.org/A134504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(7n + 6)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
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
    /* Compute the index 7*n + 6 in a type that can hold the multiplication.
       seq_t may be bigger than idx_t, so we perform the multiplication in
       seq_t and then cast back to idx_t for the fib call. */
    idx_t idx = (idx_t)((seq_t)7 * (seq_t)n + (seq_t)6);
    return fib(idx);
}
