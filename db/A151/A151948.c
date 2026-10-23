/*
 * A151948  a(n) = tau(sigma(phi(n))).
 * https://oeis.org/A151948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(σ(φ(n)))
 * Range:     u8 n <= 309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors (τ) of the sum of divisors (σ) of Euler's totient (φ) of n. */

#include "../utils/phi.h"       /* static seq_t phi(idx_t m) */
#include "../utils/sigma_of.h"  /* static seq_t sigma_of(idx_t n) */
#include "../utils/factor_u64.h"/* void factor_u64(uint64_t n, uint64_t primes[], uint64_t exps[], size_t *cnt) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* defined only for n ≥ 1 */

    /* φ(n) */
    seq_t phi_val = phi(n);                 /* phi is defined for idx_t input */

    /* σ(φ(n)) */
    seq_t sigma_val = sigma_of((idx_t)phi_val);

    /* Factor σ(φ(n)) — assume it fits into uint64_t for the tested range */
    uint64_t sigma = (uint64_t)sigma_val;

    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(sigma, primes, exps, &cnt);

    /* τ(σ(φ(n))) = product over (exponent + 1) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau = tau * (seq_t)(exps[i] + 1);

    return tau;
}
