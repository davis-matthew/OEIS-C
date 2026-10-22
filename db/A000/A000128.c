/*
 * A000128  A nonlinear binomial sum.
 * https://oeis.org/A000128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+4) - n*(n+1)/2 - 3, with F(n) = A000045(n), F(0) = 0,
 *            F(1) = 1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t f0 = 0, f1 = 1, t, tri;
    idx_t i;
    /* compute F(n+4) iteratively: after k steps f0 = F(k) */
    for (i = 0; i < n + 4; i++) {
        t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    /* n*(n+1)/2 without overflowing before the division */
    if (n % 2 == 0)
        tri = (seq_t)(n / 2) * (seq_t)(n + 1);
    else
        tri = (seq_t)n * (seq_t)((n + 1) / 2);
    return f0 - tri - 3;
}
