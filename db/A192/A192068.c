/*
 * A192068  a(n) = Fibonacci(2*n) - (n mod 2).
 * https://oeis.org/A192068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n) - (n mod 2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-01)
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
    /* Sequence starts at offset 1; behaviour for n < 1 is irrelevant. */
    idx_t m = n << 1;            /* m = 2 * n */
    seq_t fn = fib(m);           /* Fibonacci(2*n) */
    if (n & 1)                   /* if n is odd subtract 1 */
        fn -= 1;
    return fn;
}
