/*
 * A023848  Sum of exponents in prime-power factorization of binomial(5n,
 *          2n-1).
 * https://oeis.org/A023848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A023847(n) - A001222(3*n+1) + A001222(2*n) where
 *            A023847(n) = sum of exponents in the prime factorisation of
 *            binomial(5*n,3*n). The value is returned as seq_t
 * Range:     u8 n <= 483, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset: a(1) is first term */

    /* ---- compute bigomega of binomial(5n,3n) using Legendre's formula ---- */
    idx_t limit = 5 * n;               /* upper bound for primes needed   */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t binom_bigomega = 0;

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p = primes[idx];

        /* exponent of p in 5n!      */
        idx_t exp5 = 0;
        idx_t t = 5 * n;
        while (t) { t /= p; exp5 += t; }

        /* exponent of p in 3n!      */
        idx_t exp3 = 0;
        t = 3 * n;
        while (t) { t /= p; exp3 += t; }

        /* exponent of p in 2n!      */
        idx_t exp2 = 0;
        t = 2 * n;
        while (t) { t /= p; exp2 += t; }

        seq_t e = (seq_t)(exp5 - exp3 - exp2);   /* never negative */
        binom_bigomega += e;
    }
    free(primes);

    /* ---- add the remaining terms using the provided bigomega function ---- */
    seq_t bigomega_2n   = bigomega(2 * n);
    seq_t bigomega_3n1  = bigomega(3 * n + 1);

    return binom_bigomega - bigomega_3n1 + bigomega_2n;
}
