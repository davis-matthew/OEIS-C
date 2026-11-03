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
 * Validated: standard, pass, 112 terms through n = 686 (2026-11-01)
 * Helpers:   num_divisors, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* Compute τ(n) – number of divisors of n */
    seq_t tau_n = num_divisors((uint64_t)n);

    /* Compute φ(τ(n)) – Euler's totient of τ(n) */
    seq_t phi_val = totient_seq(tau_n);

    /* Compute τ(φ(τ(n))) – number of divisors of φ(τ(n)) */
    seq_t tau_phi = num_divisors((uint64_t)phi_val);

    return tau_phi;
}
