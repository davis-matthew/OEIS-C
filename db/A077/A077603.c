/*
 * A077603  Least k >= 2 such that tau(n) divides tau(n^k), where tau(n) is
 *          the number of divisors of n, A000005.
 * https://oeis.org/A077603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 2 such that τ(n) | τ(n^k) where τ is the
 *            divisor count function
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == (idx_t)1)
        return (seq_t)2;

    /* Factor n: primes[i]^{exps[i]} */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute τ(n) = ∏ (e_i + 1) */
    seq_t tau = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        tau = tau * (seq_t)(exps[i] + 1);

    /* Search for the minimal k ≥ 2 with τ(n) | τ(n^k) */
    for (seq_t k = (seq_t)2; ; ++k) {
        unsigned int prod = 1;           /* product modulo τ(n) */
        unsigned int mod_tau = (unsigned int)tau; /* τ(n) fits in unsigned int (≤ 128 for n ≤ 10000) */

        for (size_t i = 0; i < cnt; ++i) {
            /* term = (k·e_i + 1) mod τ(n) */
            unsigned int term = ((unsigned int)k * (unsigned int)exps[i] + 1) % mod_tau;
            prod = (prod * term) % mod_tau;
        }

        if (prod == 0)
            return k; /* τ(n) divides τ(n^k) */
    }
}
