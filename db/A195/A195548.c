/*
 * A195548  Numerators b(n) of Pythagorean approximations b(n)/a(n) to 1/2.
 * https://oeis.org/A195548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (F(n+1)^2 - F(n)^2) / (2 - ((n+2)^2 mod 3)), where F(k) =
 *            Fibonacci(k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
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
    seq_t fn     = fib(n);
    seq_t fnp1   = fib(n + 1);
    seq_t diff   = fnp1 * fnp1 - fn * fn;
    idx_t t      = (n + 2) * (n + 2);
    seq_t denom  = (seq_t)(2 - (t % 3));
    return diff / denom;
}
