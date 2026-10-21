/*
 * A051123  a(n) = Fibonacci(n) OR Fibonacci(n+1).
 * https://oeis.org/A051123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = fib(n) | fib(n+1)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t f0 = 0;      /* fib(0) */
    seq_t f1 = 1;      /* fib(1) */
    for (idx_t i = 0; i < n; ++i) {
        seq_t tmp = f0 + f1;   /* next Fibonacci number */
        f0 = f1;
        f1 = tmp;
    }
    return f0 | f1;   /* fib(n) OR fib(n+1) */
}
