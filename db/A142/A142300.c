/*
 * A142300  Primes congruent to 19 mod 44.
 * https://oeis.org/A142300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 19 (mod 44)
 * Range:     u8 n <= 4, u16 n <= 334, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Return the n‑th prime that is congruent to 19 modulo 44.
 * n is 1‑based (a(1) = 19). The function does not keep any cache.
 * The estimation for the sieve upper bound is conservative enough for
 * all requested n (up to about 10000) and works for both C and C++ builds.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* Undefined for n < 1, harmless */

    /* Estimate a limit large enough to contain n such primes.
     * For n ≤ 10000 this upper bound is far above the required value.   */
    idx_t limit = (idx_t)(20 * 44) * n;          /* 880 * n ≈ 8.8·10⁶ for n=10000 */
    limit += 100;                                /* safety margin      */

    while (true) {
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;

        build_primes_upto(limit, &primes, &prime_cnt);

        /* Scan the primes and count those ≡ 19 (mod 44). */
        idx_t count = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if ((primes[i] % 44ULL) == 19ULL) {
                ++count;
                if (count == n) {
                    seq_t result = (seq_t)primes[i];
                    free(primes);
                    return result;
                }
            }
        }

        /* Not enough primes found – increase the search range. */
        free(primes);
        limit <<= 1;          /* double */
    }
}
