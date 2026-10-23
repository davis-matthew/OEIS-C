/*
 * A141465  Prime transformed products of prime factors of the composites, the
 *          largest prime decremented by 2, the smallest by 1.
 * https://oeis.org/A141465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   factor_u64, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141465: a(n) is the nth prime obtained by transforming the
   prime factors of the n-th composite number according to the
   definition in the OEIS entry. The algorithm scans successive
   composite integers, computes their smallest and largest prime
   factors, applies the transformation
       k * (pmax-2) * (pmin-1) / (pmin * pmax),
   and returns the nth prime value produced. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime.h"        /* is_prime(idx_t) */
#include "../utils/is_prime_big.h"    /* is_prime_big(seq_t) */
#include "../utils/factor_u64.h"      /* factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n=0, but return 0 safely */

    idx_t  count = 0;                  /* number of valid primes found */
    uint64_t k = 4;                    /* start from the first composite */

    /* buffers for factor_u64: 64 prime factors + exponents suffice for 64‑bit numbers */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t   num_factors;

    for (;; ++k)
    {
        if (is_prime(k)) continue;     /* skip primes, we only need composites */

        /* factorization to obtain smallest and largest prime factors */
        factor_u64(k, primes, exponents, &num_factors);
        uint64_t pmin = primes[0];
        uint64_t pmax = primes[num_factors - 1];

        uint64_t denom = pmin * pmax;
        if (denom == 0) continue;      /* safety, should never happen */

        /* compute product = k / (pmin*pmax) * (pmax-2)*(pmin-1) */
        seq_t t = (seq_t)(k / denom);
        seq_t prod = t;
        prod *= (seq_t)(pmax - 2);
        prod *= (seq_t)(pmin - 1);

        /* if the result is a prime, record it */
        if (prod >= 2 && is_prime_big(prod))
        {
            ++count;
            if (count == n) return prod;
        }
    }
}
