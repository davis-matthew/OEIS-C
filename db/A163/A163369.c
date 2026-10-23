/*
 * A163369  a(n) = sigma(sigma(phi(n))).
 * https://oeis.org/A163369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(sigma(phi(n)))
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A163369 */

/* Include the utility functions for phi and sigma */
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Compute φ(n) */
    seq_t phi = calc_phi((seq_t)n);

    /* Compute σ(φ(n)) */
    idx_t phi_int = (idx_t)phi;          /* safe cast for n up to 2^64-1 */
    seq_t sigma_phi = sigma_of(phi_int);

    /* Compute σ(σ(φ(n))) */
    idx_t sigma_phi_int = (idx_t)sigma_phi; /* safe cast */
    seq_t result = sigma_of(sigma_phi_int);

    return result;
}
