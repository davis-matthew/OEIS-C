/*
 * A163376  a(n) = phi(tau(phi(n))).
 * https://oeis.org/A163376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(tau(phi(n)))
 * Range:     u8 n <= 340, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses trial‑division factoring of n and phi(n) to compute the
   totient and divisor count.  For n up to about 10000 it runs well below 2.0
   seconds per call and works for all n that fit in an idx_t. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute φ(m) where m is an idx_t (≤ 2^64−1).  Returns a seq_t. */
static seq_t phi_small(idx_t m)
{
    if (m <= 1) return (seq_t)1;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);
    idx_t result = m;
    for (size_t i = 0; i < cnt; ++i) {
        result = result / primes[i] * (primes[i] - 1);
    }
    return (seq_t)result;
}

/* Compute τ(m) (number of divisors) for an idx_t m.  Returns an idx_t. */
static idx_t tau_small(idx_t m)
{
    if (m == 0) return 0;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);
    idx_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        result *= (exps[i] + 1);
    }
    return result;
}

/* Main sequence function.  a(1) = 1, a(2) = 1, etc. */
seq_t a(idx_t n)
{
    seq_t phi_n = phi_small(n);          /* φ(n) */
    idx_t phi_val = (idx_t)phi_n;        /* φ(n) fits in idx_t */
    idx_t tau_phi = tau_small(phi_val);  /* τ(φ(n)) */
    return phi_small((idx_t)tau_phi);    /* φ(τ(φ(n))) */
}
