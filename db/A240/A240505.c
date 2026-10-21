/*
 * A240505  Products of primes the squares of which are Fermi-Dirac divisors
 *          of n!
 * https://oeis.org/A240505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p ≤ n, v_p(n!) ≡ 2 or 3 (mod 4)} p where v_p(n!) is
 *            the exponent of prime p in n!
 * Range:     u8 n <= 23, u16 n <= 37, u32 n <= 65, u64 n <= 139,
 *            u128 n <= 265, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* Return 1 for n < 1 (unused, but safe) */
    if (n < 1u)
        return (seq_t)1;

    idx_t *primes = NULL;
    size_t prime_count = 0;
    build_primes(n, &primes, &prime_count);

    seq_t prod = (seq_t)1;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        idx_t t = p;
        unsigned e_mod4 = 0;

        while (t <= n) {
            e_mod4 = (e_mod4 + (unsigned)(n / t)) & 3U;

            /* Prevent overflow on the next multiplication */
            if (t > n / p)
                break;
            t *= p;
        }

        if (e_mod4 == 2u || e_mod4 == 3u) {
            prod = prod * (seq_t)p;
        }
    }

    free(primes);
    return prod;
}
