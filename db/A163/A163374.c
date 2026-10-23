/*
 * A163374  a(n) = tau(tau(phi(n))).
 * https://oeis.org/A163374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(tau(phi(n)))
 * Range:     u8 n <= 283, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Euler's totient of n */
    seq_t phi_n = phi(n);

    /* Count divisors of phi(n) */
    uint64_t phi_val = (uint64_t)phi_n;
    seq_t tau_phi = divisor_count_u64(phi_val);

    /* Count divisors of tau(phi(n)) */
    uint64_t tau_phi_val = (uint64_t)tau_phi;
    seq_t result = divisor_count_u64(tau_phi_val);

    return result;
}
