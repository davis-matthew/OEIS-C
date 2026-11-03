/*
 * A081005  a(n) = Fibonacci(4n+3) + 1, or Fibonacci(2n+1)*Lucas(2n+2).
 * https://oeis.org/A081005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n + 3) + 1 (OEIS A081005)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 471 (2026-11-01)
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
    /* Compute the Fibonacci index 4*n+3 */
    idx_t fib_index = n * 4 + 3;

    /* Fibonacci value at that index */
    seq_t f = fib(fib_index);

    /* Return the sequence term */
    return f + (seq_t)1;
}
