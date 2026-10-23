/*
 * A163372  a(n) = sigma(phi(sigma(n))).
 * https://oeis.org/A163372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(phi(sigma(n)))
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* Step 1: sigma(n) */
    seq_t s1 = sigma_of(n);

    /* Step 2: phi(sigma(n)) */
    seq_t s2 = calc_phi(s1);

    /* Step 3: sigma(phi(sigma(n))) */
    return sigma_of((idx_t)s2);
}
