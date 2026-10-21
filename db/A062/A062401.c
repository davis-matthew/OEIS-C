/*
 * A062401  a(n) = phi(sigma(n)).
 * https://oeis.org/A062401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(sigma(n)) for n ≥ 1; offset = 1
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1. For n < 1 the result is undefined; return 0 safely */
    if (n < 1)
        return 0;

    seq_t sigma = sigma_of(n);   /* compute σ(n) */
    return calc_phi(sigma);      /* compute φ(σ(n)) */
}
