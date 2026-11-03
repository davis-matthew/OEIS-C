/*
 * A137976  Fibonacci numbers (A000045) not in A103311.
 * https://oeis.org/A137976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fib((5*n + 4 - (n & 1)) / 2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
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
    /* Compute the index k of the Fibonacci number to return.
       For odd n: k = (5*n + 3)/2
       For even n: k = (5*n + 4)/2   */
    idx_t k = (5 * n + 4 - (n & 1)) >> 1;
    return fib(k);
}
