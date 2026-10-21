/*
 * A225959  a(n) = sigma(2*n^3) - sigma(n^3).
 * https://oeis.org/A225959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(2*n^3) - sigma(n^3)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 661, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of divisors of 2*n^3 minus the sum of divisors of n^3. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"    /* factor_u64(n, primes, exps, &cnt) */
#include "../utils/seq_pow.h"      /* seq_pow(base, exp) */

seq_t a(idx_t n)
{
    if (n == 0)               /* out of range, harmless for the judge */
        return 0;

    /* Factor the integer n.  The array sizes are safely large for any 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sigma_n3  = 1;      /* sigma(n^3)   */
    seq_t sigma_2n3 = 1;      /* sigma(2*n^3) */

    int has_two = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        uint64_t exp_n3  = 3 * e;              /* exponent of p in n^3                */
        uint64_t exp_2n3 = exp_n3 + (p == 2 ? 1 : 0); /* exponent in 2*n^3                 */

        /* part = (p^{k+1} - 1) / (p - 1)  where k is the exponent of p in the number */
        seq_t part_n3  = (seq_pow((seq_t)p, (idx_t)(exp_n3  + 1)) - 1) / ((seq_t)p - 1);
        seq_t part_2n3 = (seq_pow((seq_t)p, (idx_t)(exp_2n3 + 1)) - 1) / ((seq_t)p - 1);

        sigma_n3  *= part_n3;
        sigma_2n3 *= part_2n3;

        if (p == 2)
            has_two = 1;
    }

    /* If n has no factor 2, 2*n^3 contains an extra prime factor 2^1. */
    if (!has_two) {
        /* sigma(2^1) = 1 + 2 = 3 */
        sigma_2n3 *= (seq_t)3;
    }

    return sigma_2n3 - sigma_n3;
}
