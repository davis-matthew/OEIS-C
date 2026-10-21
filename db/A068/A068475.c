/*
 * A068475  a(n) = Sum_{m=0..n} m*n^(m-1).
 * https://oeis.org/A068475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=0..n} m*n^(m-1) (OEIS A068475)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t res   = (seq_t)0;   /* accumulator for the sum */
    seq_t power = (seq_t)1;   /* current value of n^(m-1) */

    for (idx_t m = 1; m <= n; ++m) {
        /* term = m * n^(m-1) */ 
        res += (seq_t)m * power;
        /* prepare n^m for next iteration */
        power *= (seq_t)n;
    }

    return res;
}
