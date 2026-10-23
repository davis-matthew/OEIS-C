/*
 * A163377  a(n) = tau(phi(tau(n))).
 * https://oeis.org/A163377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(phi(tau(n))) where tau is the divisor count and phi
 *            is Euler's totient
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* Compute τ(n) – number of divisors of n */
    seq_t tau_n = divisor_count_u64((uint64_t)n);

    /* Compute φ(τ(n)) – Euler's totient of τ(n) */
    seq_t phi_val = calc_phi(tau_n);

    /* Compute τ(φ(τ(n))) – number of divisors of φ(τ(n)) */
    seq_t tau_phi = divisor_count_u64((uint64_t)phi_val);

    return tau_phi;
}
