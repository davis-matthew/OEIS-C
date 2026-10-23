/*
 * A163373  a(n) = phi(phi(sigma(n))).
 * https://oeis.org/A163373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(phi(sigma(n)))
 * Range:     u8 n <= 278, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);
    seq_t p1 = calc_phi(s);
    seq_t p2 = calc_phi(p1);
    return p2;
}
