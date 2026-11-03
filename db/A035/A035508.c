/*
 * A035508  a(n) = Fibonacci(2*n+2) - 1.
 * https://oeis.org/A035508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+2) - 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
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
    /* Compute Fibonacci(2*n+2) using the provided Fibonacci routine. */
    idx_t k = (n << 1) + 2;     /* 2*n + 2, safe in unsigned 64-bit */
    seq_t fn = fib(k);          /* Fibonacci(k) */
    return fn - 1;              /* subtract 1 to obtain a(n) */
}
