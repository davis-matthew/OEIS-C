/*
 * A215045  a(n) = F(2*n+1)^5 with n >= 0, F=A000045 (Fibonacci numbers).
 * https://oeis.org/A215045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(2*n+1)^5
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 179 (2026-11-01)
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
    /* compute Fibonacci number F(2*n+1) */
    idx_t m = (idx_t)((seq_t)n * 2 + 1);
    seq_t f = fib(m);

    /* compute f^5 */
    seq_t res = 1;
    for (int i = 0; i < 5; ++i)
        res *= f;

    return res;
}
