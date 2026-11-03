/*
 * A022394  Fibonacci sequence beginning 1, 24.
 * https://oeis.org/A022394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) + a(n‑2); a(0) = 1, a(1) = 24
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 989 (2026-11-01)
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
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    /* Fibonacci numbers F(n) with F(0)=0, F(1)=1 */
    seq_t fn  = fib(n);     /* F(n)   */
    seq_t fn1 = fib(n-1);   /* F(n-1) */
    return (seq_t)1 * fn1 + (seq_t)24 * fn;
}
