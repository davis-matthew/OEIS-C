/*
 * A153873  a(n) = 9*Fibonacci(2n+1) - 1.
 * https://oeis.org/A153873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * Fibonacci(2n + 1) - 1 Recurrence: a(n) = 3*a(n-1) -
 *            a(n-2) + 1, a(0)=8, a(1)=17 (Either form can be used; this
 *            implementation uses the Fibonacci formula)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 193 (2026-11-01)
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
    /* Compute Fibonacci(2n + 1) */
    idx_t m = (idx_t)2 * n + (idx_t)1;
    seq_t f = fib(m);

    /* a(n) = 9*f - 1 */
    seq_t res = f * (seq_t)9;
    res -= (seq_t)1;
    return res;
}
