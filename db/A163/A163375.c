/*
 * A163375  a(n) = sigma(tau(phi(n))).
 * https://oeis.org/A163375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(tau(phi(n)))
 * Range:     u8 n <= 332, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n-th term of OEIS sequence A163375.
 * a(n) = sigma(tau(phi(n))) where
 *   phi(n)   = Euler's totient function,
 *   tau(m)   = number of divisors of m,
 *   sigma(t) = sum of divisors of t. */

#include "../utils/calc_phi.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Compute φ(n) */
    seq_t phi = calc_phi((seq_t)n);

    /* divisor_count_u64 expects a uint64_t argument */
    idx_t phi_u = (idx_t)phi;
    idx_t d = divisor_count_u64(phi_u);

    /* Return σ(d) */
    return sigma_of(d);
}
