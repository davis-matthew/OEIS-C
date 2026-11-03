/*
 * A094967  Right-hand neighbors of Fibonacci numbers in Stern's diatomic
 *          series.
 * https://oeis.org/A094967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n)*(1-(-1)^n)/2 + Fibonacci(n+1)*(1+(-1)^n)/2
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Validated: standard, pass, 58 terms through n = 1892 (2026-11-01)
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
    /* For even n: a(n) = F_{n+1}; for odd n: a(n) = F_n */
    if ((n & 1U) == 0U)
        return fib(n + 1);
    else
        return fib(n);
}
