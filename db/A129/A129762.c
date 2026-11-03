/*
 * A129762  Sum of all elements of n X n X n cubic array M[i,j,k] =
 *          Fibonacci[i+j+k-2].
 * https://oeis.org/A129762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(3*n+4) - 3*Fibonacci(2*n+4) + 3*Fibonacci(n+4)
 *            - 3 (Fibonacci(0)=0, Fibonacci(1)=1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
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
    /* compute the three Fibonacci terms */
    seq_t f3n4 = fib(3*n + 4);
    seq_t f2n4 = fib(2*n + 4);
    seq_t fn4  = fib(n + 4);

    /* apply the formula using integer arithmetic on seq_t */
    seq_t res = f3n4;
    res -= f2n4 * 3;
    res += fn4 * 3;
    res -= 3;

    return res;
}
