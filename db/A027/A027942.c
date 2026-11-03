/*
 * A027942  a(n) = T(2n+1, n+2), T given by A027935.
 * https://oeis.org/A027942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+5) - 2*n^2 - 5*n - 5
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 964 (2026-11-01)
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
    /* convert index to seq_t */
    seq_t n_seq = (seq_t)n;

    /* compute Fibonacci(2*n+5) */
    idx_t k = 2 * n + 5;
    seq_t fib_val = fib(k);

    /* compute subtrahend: 2 * n^2 + 5 * n + 5 */
    seq_t sub = 2 * n_seq * n_seq + 5 * n_seq + 5;

    /* final result */
    return fib_val - sub;
}
