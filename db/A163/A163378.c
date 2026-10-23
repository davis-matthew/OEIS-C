/*
 * A163378  a(n) = sigma(phi(tau(n))).
 * https://oeis.org/A163378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(phi(tau(n)))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* τ(n) – number of divisors of n (as a seq_t) */
    seq_t tau = (seq_t)divisor_count_u64((uint64_t)n);

    /* φ(τ(n)) – Euler's totient of τ(n) (returns a seq_t) */
    seq_t phi = calc_phi(tau);

    /* σ(φ(τ(n))) – sum of divisors of φ(τ(n)) */
    /* phi is known to be small enough to fit into idx_t for the
       ranges required in the tests, so we cast it safely. */
    idx_t phi_idx = (idx_t)phi;
    seq_t sigma = sigma_of(phi_idx);

    return sigma;
}
