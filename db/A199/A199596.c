/*
 * A199596  a(n) = floor(phi(2*n+1)/phi(2*n+2)).
 * https://oeis.org/A199596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(phi(2*n+1)/phi(2*n+2))
 * Range:     u8 n <= 184, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* 2*n can overflow if n is large, so cast to seq_t before multiplying */
    seq_t m1 = (seq_t)n * 2 + 1;
    seq_t m2 = (seq_t)n * 2 + 2;
    seq_t phi1 = calc_phi(m1);
    seq_t phi2 = calc_phi(m2);
    return phi1 / phi2;   /* integer division = floor for positive numbers */
}
