/*
 * A163371  a(n) = tau(phi(sigma(n))).
 * https://oeis.org/A163371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(phi(sigma(n)))
 * Range:     u8 n <= 321, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi, divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors of φ(σ(n)) for n ≥ 1 */
#include "../utils/sigma_of.h"    /* seq_t sigma_of(idx_t n) */
#include "../utils/calc_phi.h"   /* seq_t calc_phi(seq_t n) */
#include "../utils/divisor_count_u64.h" /* uint64_t divisor_count_u64(uint64_t n) */

seq_t a(idx_t n)
{
    /* σ(n) */
    seq_t sigma = sigma_of(n);

    /* φ(σ(n)) */
    seq_t phi_val = calc_phi(sigma);

    /* tau(φ(σ(n))) – φ(σ(n)) fits in 64 bits for the range considered */
    uint64_t phi_u = (uint64_t)phi_val;
    uint64_t tau_val = divisor_count_u64(phi_u);

    return (seq_t)tau_val;
}
