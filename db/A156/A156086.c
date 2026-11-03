/*
 * A156086  Sum of the squares of the first n Fibonacci numbers with index
 *          divisible by 4.
 * https://oeis.org/A156086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(8*n+4) - 3*(2*n+1)) / 15
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the Fibonacci function from utils/fib.h */

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
    /* compute m = 8*n + 4 */
    idx_t m = n * 8 + 4;

    /* F(8*n+4) */
    seq_t f = fib(m);

    /* sub = 3*(2*n+1) */
    idx_t two_n_plus_1 = n * 2 + 1;
    seq_t sub = (seq_t)(two_n_plus_1 * 3);

    /* numerator = F(8*n+4) - 3*(2*n+1) */
    seq_t numerator = f - sub;

    /* divide by 15 */
    return numerator / 15;
}
