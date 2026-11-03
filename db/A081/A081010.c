/*
 * A081010  a(n) = Fibonacci(4n+1) + 2, or Fibonacci(2n-1)*Lucas(2n+2).
 * https://oeis.org/A081010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(4*n+1) + 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 468 (2026-11-01)
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
    idx_t k = 4 * n + 1;   /* 4*n+1 fits in idx_t for all expected n */
    seq_t val = fib(k);    /* compute Fibonacci(k) */
    val += (seq_t)2;       /* add 2 */
    return val;
}
