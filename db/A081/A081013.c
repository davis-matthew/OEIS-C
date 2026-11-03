/*
 * A081013  a(n) = Fibonacci(4*n+3) - 2, or Fibonacci(2*n)*Lucas(2*n+3).
 * https://oeis.org/A081013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n+3) - 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 999 (2026-11-01)
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
    /* Compute the index 4*n+3 in a type safe manner */
    idx_t idx = n << 2;   /* multiply by 4 */
    idx += 3;             /* add 3 */

    /* fib takes an idx_t and returns Fibonacci(idx_t) as seq_t */
    seq_t f = fib(idx);

    /* Subtract 2 (converted to seq_t) */
    return f - (seq_t)2;
}
