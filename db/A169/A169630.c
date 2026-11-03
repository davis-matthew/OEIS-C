/*
 * A169630  a(n) = n times the square of Fibonacci(n).
 * https://oeis.org/A169630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(Fib(n))^2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 989 (2026-11-01)
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
    seq_t fib_n = fib(n);              /* Fib(n) in seq_t */
    seq_t n_seq = (seq_t)n;            /* n converted to seq_t */
    return n_seq * fib_n * fib_n;      /* n * (Fib(n))^2 */
}
