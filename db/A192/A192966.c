/*
 * A192966  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+4) + 2*Fibonacci(n+2) - (n^2 + 5*n + 10)/2
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 388 (2026-11-01)
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
    seq_t fn4   = fib(n + 4);          /* Fibonacci(n+4) */
    seq_t fn2   = fib(n + 2);          /* Fibonacci(n+2) */
    seq_t sum_fib = fn4 + (fn2 << 1);  /* Fib(n+4) + 2*Fib(n+2) */

    /* (n^2 + 5*n + 10)/2  – integer division truncates, the numerator is always even */
    seq_t half_sum = ((seq_t)n * (seq_t)n + (seq_t)5 * n + (seq_t)10) / 2;

    return sum_fib - half_sum;
}
