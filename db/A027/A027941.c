/*
 * A027941  a(n) = Fibonacci(2*n + 1) - 1.
 * https://oeis.org/A027941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Fibonacci(2*n + 1) - 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 182 (2026-11-01)
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
    /* compute the index of the Fibonacci number: 2*n + 1 */
    idx_t k = n + n + 1;      /* safe for all n in the range requested */
    seq_t f = fib(k);         /* fib(k) returns Fibonacci(k) in seq_t */
    f -= 1;                   /* a(n) = Fib(2*n + 1) - 1 */
    return f;
}
