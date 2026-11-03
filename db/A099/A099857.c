/*
 * A099857  Expansion of (1+3*x+x^2)/(1-3*x+x^2).
 * https://oeis.org/A099857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0^n + 6*Fib(2*n) (with 0^0 = 1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 2365 (2026-11-01)
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
    if (n == 0) {
        return (seq_t)1;           /* 0^0 = 1, Fib(0) = 0, so a(0)=1 */
    }
    /* For n > 0, 0^n = 0, so a(n) = 6 * Fib(2*n) */
    return (seq_t)6 * fib(2 * n);
}
