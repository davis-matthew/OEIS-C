/*
 * A027938  a(n) = T(2n, n+2), T given by A027935.
 * https://oeis.org/A027938
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+5) - (4*n^3 + 6*n^2 + 14*n + 15)/3
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 968 (2026-11-01)
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
    /* The sequence is defined for n >= 2 (offset 2). */
    if (n < 2u) {
        return (seq_t)0;
    }

    /* Fibonacci part */
    seq_t fib_val = fib(2 * n + 5);

    /* Polynomial part: (4*n^3 + 6*n^2 + 14*n + 15)/3 */
    seq_t n_seq  = (seq_t)n;
    seq_t n_sq   = n_seq * n_seq;              /* n^2 */
    seq_t n_cu   = n_sq  * n_seq;              /* n^3 */
    seq_t poly   = 4 * n_cu + 6 * n_sq + 14 * n_seq + 15;
    poly /= 3;                                 /* division is exact */

    /* Result */
    return fib_val - poly;
}
