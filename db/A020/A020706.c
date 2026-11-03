/*
 * A020706  Pisot sequences L(4,6), E(4,6).
 * https://oeis.org/A020706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fib(n+4) + 1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 917 (2026-11-01)
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
    /* Compute the (n+4)th Fibonacci number and add 1. */
    return fib(n + 4) + 1;
}
