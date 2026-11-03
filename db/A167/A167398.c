/*
 * A167398  a(n) = Fibonacci(11*n).
 * https://oeis.org/A167398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Fibonacci(11 * n)
 * (OEIS A167398)
 *
 * Fibonacci is assumed to be 0‑indexed: fib(0)=0, fib(1)=1, fib(2)=1, …
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
    /* Special case for the first term a(0) = fib(0) = 0 */
    if (n == 0)
        return 0;

    /* For n > 0 compute fib(11*n) */
    return fib(n * 11);
}
