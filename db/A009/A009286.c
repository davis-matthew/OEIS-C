/*
 * A009286  a(n) = lcm(sigma(n), phi(n)).
 * https://oeis.org/A009286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(σ(n), φ(n))
 * Range:     u8 n <= 22, u16 n <= 288, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Compute σ(n) and φ(n) using the utility functions */
    seq_t sigma_n = sigma_of(n);
    seq_t phi_n   = phi(n);

    /* lcm(σ,φ) = σ/ gcd(σ,φ) * φ */
    seq_t g      = gcd(sigma_n, phi_n);
    seq_t part   = sigma_n / g;   /* exact division */
    return part * phi_n;
}
