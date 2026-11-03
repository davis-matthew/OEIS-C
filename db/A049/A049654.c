/*
 * A049654  a(n) = (F(8*n+1) - 1)/3 where F=A000045 (the Fibonacci sequence).
 * https://oeis.org/A049654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(8*n+1) - 1) / 3, where F = Fibonacci sequence
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 595 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    idx_t m = n * 8 + 1;          /* 8*n + 1  (fits in idx_t for tested ranges) */
    seq_t f = fib(m);            /* compute F(8*n+1) */
    f -= 1;
    f /= 3;
    return f;
}
