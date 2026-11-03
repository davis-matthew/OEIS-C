/*
 * A054888  Layer counting sequence for hyperbolic tessellation by regular
 *          pentagons of angle Pi/2.
 * https://oeis.org/A054888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*Fib(2n) for n >= 1, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 958 (2026-11-01)
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
    if (n == 0) return (seq_t)1;
    idx_t k = n << 1;                // k = 2*n
    return (seq_t)5 * fib(k);       // a(n) = 5 * Fib(2n)
}
