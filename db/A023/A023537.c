/*
 * A023537  a(n) = Lucas(n+4) - (3*n+7).
 * https://oeis.org/A023537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n+4) - (3*n+7)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 979 (2026-11-01)
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
    seq_t n_seq = (seq_t)n;
    /* compute Lucas(n+4) = F(n+3) + F(n+5) */
    seq_t fn3 = fib(n + 3);
    seq_t fn5 = fib(n + 5);
    seq_t val = fn3 + fn5;

    /* subtract (3*n + 7) */
    val -= (seq_t)(3) * n_seq + (seq_t)7;

    return val;
}
