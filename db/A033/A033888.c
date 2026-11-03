/*
 * A033888  a(n) = Fibonacci(4*n).
 * https://oeis.org/A033888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n) a(0) = 0, a(1) = 3, a(2) = 21, …
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 281 (2026-11-01)
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
    /* The helper fib() seems to return 1 for fib(0), while
       a(0) must be 0.  Treat the zero‑index specially, otherwise
       fib(4*n) yields the correct term for all n ≥ 1. */
    if (n == 0)
        return 0;
    return fib(n << 2);       /* equivalent to fib(4*n) */
}
