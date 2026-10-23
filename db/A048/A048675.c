/*
 * A048675  If n = p_i^e_i * ... * p_k^e_k, p_i < ... < p_k primes (with p_i =
 *          prime(i)), then a(n) = (1/2) * (e_i * 2^i + ... + e_k * 2^k).
 * https://oeis.org/A048675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (1/2) * Σ_{p^e || n} e * 2^PrimeIndex(p) Equivalently
 *            a(p^e) = e * 2^(PrimePi(p)-1) (PrimePi is A000720)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Helpers:   factor_u64, mul_small, pow2, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prime_pi.h"
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    /* factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* index of prime p (1‑based) */
        idx_t pi = (idx_t)prime_pi(p);

        /* contribution = e * 2^(pi-1) */
        seq_t term = pow2(pi - 1);
        if (e > 1) term = mul_small(term, (unsigned int)e);

        sum += term;
    }
    return sum;
}
