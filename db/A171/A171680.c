/*
 * A171680  a(n) = F(2*n)^3 - F(3*n-1)^2 - F(6*n-8).
 * https://oeis.org/A171680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = fibonacci(2*n)^3 - fibonacci(3*n-1)^2 - fibonacci(6*n-8)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 17, i128 n <= 32,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

static seq_t fib_signed(int64_t n)
{
    if (n >= 0)
        return fib((idx_t)n);

    /* Fibonacci of a negative index: F(-k) = (-1)^{k+1} * F(k) */
    int64_t k = -n;
    seq_t prod = fib((idx_t)k);
    if ((k & 1) == 0) /* even k -> sign = -1 */
        return -prod;
    return prod;      /* odd k -> sign = +1 */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* safe guard for non‑positive indices */

    /* Compute required Fibonacci numbers */
    seq_t f2   = fib(2 * n);
    seq_t f3   = fib_signed((int64_t)3 * (int64_t)n - 1);
    seq_t f6   = fib_signed((int64_t)6 * (int64_t)n - 8);

    /* f2^3 */
    seq_t f2_sq = f2 * f2;
    seq_t f2_cu = f2_sq * f2;

    /* f3^2 */
    seq_t f3_sq = f3 * f3;

    /* a(n) = f2^3 - f3^2 - f6 */
    return f2_cu - f3_sq - f6;
}
