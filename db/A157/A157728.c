/*
 * A157728  a(n) = Fibonacci(n) - 4.
 * https://oeis.org/A157728
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) - 4, n >= 5
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 266 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided fib(idx_t n) helper from the utilities. */

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
    /* For n >= 5 the value is defined.  Behaviour for smaller n is irrelevant. */
    return fib(n) - (seq_t)4;
}
