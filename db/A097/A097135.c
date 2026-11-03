/*
 * A097135  a(0) = 1; for n>0, a(n) = 3*Fibonacci(n).
 * https://oeis.org/A097135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n = 0) ; a(n) = 3 * Fibonacci(n) (n > 0)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 38 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function fib(idx_t n) defined in ../utils/fib.h */

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
    if (n == 0)
        return (seq_t)1;
    seq_t f = fib(n);
    return f * (seq_t)3;
}
