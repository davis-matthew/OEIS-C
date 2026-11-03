/*
 * A023619  Convolution of Lucas numbers and (F(2), F(3), F(4), ...).
 * https://oeis.org/A023619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*F_n + n*F_{n+1}
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 38, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 828 (2026-11-01)
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
    if (n == 0)
        return 0;                  /* undefined for n = 0 */
    seq_t fn  = fib(n);            /* F_n   */
    seq_t fn1 = fib(n + 1);        /* F_{n+1} */
    seq_t part1 = ((seq_t)(n - 1)) * fn;
    seq_t part2 = ((seq_t)n) * fn1;
    return part1 + part2;
}
