/*
 * A000126  A nonlinear binomial sum.
 * https://oeis.org/A000126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+3) - (n+1)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = 0, y = 1, t;
    idx_t i;
    for (i = 0; i < n + 3; i++) {
        t = x + y;
        x = y;
        y = t;
    }
    /* x = Fibonacci(n+3) */
    return (seq_t)(x - ((seq_t)n + 1));
}
