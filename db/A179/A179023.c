/*
 * A179023  a(n) = n(F(n+2) - 1) where F(n) is defined by A000045.
 * https://oeis.org/A179023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (F(n+2) - 1) where F is the Fibonacci sequence
 *            A000045
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 32 (2026-11-01)
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
    /* For n = 0,1 this formula automatically gives the correct value. */
    seq_t fn2 = fib(n + 2);   /* F(n+2) */
    fn2 -= (seq_t)1;          /* F(n+2) - 1 */
    seq_t result = (seq_t)n * fn2;  /* n * (F(n+2) - 1) */
    return result;
}
