/*
 * A086156  a(n) = sigma(n^2) - n*sigma(n).
 * https://oeis.org/A086156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n^2) – n*sigma(n)
 * Range:     u8 n <= 29, u16 n <= 329, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_from_factors, sigma_sq_from_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition directly by factoring n and
   computing the divisor sums for n and n^2. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/sigma_from_factors.h"
#include "../utils/sigma_sq_from_factors.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* defined only for n ≥ 1 */

    /* Factor n.  At most 64 distinct prime factors for 64‑bit n. */
    static const size_t MAX_FACTORS = 64;
    idx_t primes[MAX_FACTORS];
    int exps[MAX_FACTORS];
    size_t nfactors = 0;

    idx_t tmp = n;
    for (idx_t i = 2; i * i <= tmp; ++i) {
        if (tmp % i == 0) {
            int e = 0;
            while (tmp % i == 0) {
                tmp /= i;
                ++e;
            }
            if (nfactors < MAX_FACTORS) {
                primes[nfactors] = i;
                exps[nfactors]   = e;
                ++nfactors;
            }
        }
    }
    if (tmp > 1) {                 /* remainder is a prime factor */
        if (nfactors < MAX_FACTORS) {
            primes[nfactors] = tmp;
            exps[nfactors]   = 1;
            ++nfactors;
        }
    }

    /* Compute σ(n) and σ(n²) from the factorization. */
    seq_t sigma_n  = sigma_from_factors(primes, exps, nfactors);
    seq_t sigma_n2 = sigma_sq_from_factors(primes, exps, nfactors);

    /* a(n) = σ(n²) – n·σ(n) */
    seq_t prod = ((seq_t)n) * sigma_n;
    return sigma_n2 - prod;
}
