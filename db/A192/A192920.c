/*
 * A192920  Coefficient of x in the reduction by (x^2 -> x+1) of the
 *          polynomial F(n+4)*x^n, where F = A000045 (Fibonacci sequence).
 * https://oeis.org/A192920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+2)^2 – (–1)^n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 902 (2026-11-01)
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
    seq_t f = fib(n + 2);   /* Fibonacci(n+2) */
    seq_t sq = f * f;       /* square it */

    /* (–1)^n = 1 when n is even, –1 when n is odd */
    if ((n & 1) == 0)
        return sq - 1;      /* even n: subtract 1 */
    else
        return sq + 1;      /* odd n: add 1 */
}
