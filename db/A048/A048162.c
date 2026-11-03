/*
 * A048162  Expansion of (1 - x + 3*x^3 - 2*x^4 - 3*x^5)/(1 - 2*x + x^3).
 * https://oeis.org/A048162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(n+1) + Fib(n-1) + 2 for n > 2; a(0) = 1, a(1) = 1,
 *            a(2) = 2
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-01)
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

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    /* n >= 3 */
    return fib(n + 1) + fib(n - 1) + (seq_t)2;
}
