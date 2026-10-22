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
 * Helpers:   a241_tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a241_tri.h"

seq_t a(idx_t n)
{
    seq_t p, q;
    if (n < 4)
        return 0;
    p = (seq_t)(n / 2);
    q = (seq_t)((n - 1) / 2);
    return a241_tri(p) * a241_tri(q);
}
