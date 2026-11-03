/*
 * A180674  a(n) = Fibonacci(n+9) - Fibonacci(9).
 * https://oeis.org/A180674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+9) - Fibonacci(9)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 177,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 275 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Fibonacci(0)=0, Fibonacci(1)=1 */

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
    /* Fibonacci(9) = 34 */
    return fib(n + 9) - (seq_t)34;
}
