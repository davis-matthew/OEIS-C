/*
 * A126832  Ramanujan numbers (A000594) read mod 5.
 * https://oeis.org/A126832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * sigma(n) mod 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*     where sigma(n) is the sum of divisors of n. */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0u)   /* offset is 1; value for n==0 is undefined, return 0 */
        return 0;

    /* Factor n.  Arrays are large enough for n up to 64‑bit numbers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sigma_mod = 1;   /* product of (1 + p + p² + … + pᵉ) modulo 5 */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t pmod = (seq_t)(primes[i] % 5u);

        /* Compute term = 1 + p + p² + … + pᵉ  (mod 5) */
        seq_t term = 1;
        seq_t pow  = 1;
        idx_t exp  = (idx_t)exps[i];

        for (idx_t j = 0; j < exp; ++j) {
            pow  = (pow * pmod) % 5u;
            term = (term + pow) % 5u;
        }

        sigma_mod = (sigma_mod * term) % 5u;
    }

    seq_t nmod = (seq_t)(n % 5u);
    return (nmod * sigma_mod) % 5u;
}
