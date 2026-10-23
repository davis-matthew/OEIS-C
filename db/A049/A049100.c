/*
 * A049100  a(n) = Euler phi function applied 4 times to n.
 * https://oeis.org/A049100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(phi(phi(phi(n))))
 * Range:     u8 n <= 728, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = calc_phi(x);
    x = calc_phi(x);
    x = calc_phi(x);
    x = calc_phi(x);
    return x;
}
