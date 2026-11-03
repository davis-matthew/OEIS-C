/*
 * A000241  Crossing number of complete graph with n nodes.
 * https://oeis.org/A000241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, nice
 * Formula:   a(n) = floor(n/2)*floor((n-1)/2)*floor((n-2)/2)*floor((n-3)/2)/4
 *            (Hill's formula, equal to the empirical g.f.
 *            -x^5*(1+x+x^2)/((x+1)^3*(x-1)^5))
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* m*(m-1)/2, computed without overflowing before the division */
static seq_t a241_tri(seq_t m)
{
    if (m % 2 == 0)
        return (m / 2) * (m - 1);
    return m * ((m - 1) / 2);
}

seq_t a(idx_t n)
{
    seq_t p, q;
    if (n < 4)
        return 0;
    p = (seq_t)(n / 2);
    q = (seq_t)((n - 1) / 2);
    return a241_tri(p) * a241_tri(q);
}
