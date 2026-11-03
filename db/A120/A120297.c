/*
 * A120297  Sum of all matrix elements of n X n matrix M(i,j) =
 *          Fibonacci(i+j-1).
 * https://oeis.org/A120297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+3) - 2*Fibonacci(n+3) + 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 191 (2026-11-01)
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
    /* Compute F(2*n+3) and F(n+3) */
    seq_t F_big = fib(2 * n + 3);
    seq_t F_mid = fib(n + 3);

    /* 2*F_mid = F_mid + F_mid */
    seq_t two_F_mid = F_mid + F_mid;

    /* Result = F_big - 2*F_mid + 2 */
    seq_t result = F_big - two_F_mid;
    result += 2;

    return result;
}
