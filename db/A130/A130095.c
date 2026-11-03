/*
 * A130095  Inverse Möbius transform of odd-indexed Fibonacci numbers.
 * https://oeis.org/A130095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n} Fibonacci(2*d - 1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
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
    if (n == 0) return (seq_t)0;
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            sum += fib(2 * d - 1);
            if (d != n / d)
                sum += fib(2 * (n / d) - 1);
        }
    }
    return sum;
}
