/*
 * A027974  a(n) = Sum_{k=1..n+1} A027960(n+1, n+1+k).
 * https://oeis.org/A027974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+3) - Fibonacci(n+5) - Fibonacci(n+3)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 988 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

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
    /* 2^(n+3) */
    seq_t pow = pow2((idx_t)(n + 3));

    /* Fibonacci(n+5) and Fibonacci(n+3) */
    seq_t f1 = fib((idx_t)(n + 5));
    seq_t f2 = fib((idx_t)(n + 3));

    /* a(n) = pow - f1 - f2;  pow is always >= f1 + f2 for n >= 0 */
    return pow - f1 - f2;
}
