/*
 * A013967  a(n) = sigma_19(n), the sum of the 19th powers of the divisors of
 *          n.
 * https://oeis.org/A013967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sigma_19(n) = ∑_{d|n} d^19
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 106,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Factor n: primes[i]^exps[i] */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];

        /* Compute p^19 once (may overflow for very large p,
           but such cases correspond to results that exceed seq_t). */
        seq_t p_pow19 = (seq_t)1;
        for (idx_t k = 0; k < 19; ++k)
            p_pow19 *= p;

        /* Sum 1 + p^19 + p^(2*19) + … + p^(e*19) */
        seq_t term = (seq_t)1;
        seq_t power = (seq_t)1;
        for (idx_t j = 1; j <= e; ++j) {
            power *= p_pow19;   /* now power = p^(19*j) */
            term += power;
        }

        result *= term;
    }

    return result;
}
