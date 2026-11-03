/*
 * A088305  a(0) = 1, a(n) = Fibonacci(2*n). It has the property that a(n) =
 *          1*a(n-1) + 2*a(n-2) + 3*a(n-3) + 4*a(n-4) + ...
 * https://oeis.org/A088305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(2*n); a(0) = 1 (special case)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 936 (2026-11-01)
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
    if (n == 0)
        return (seq_t)1;
    return fib(n * (idx_t)2);
}
