/*
 * A142431  Primes congruent to 20 mod 49.
 * https://oeis.org/A142431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 20 mod 49
 * Range:     u8 n <= 1, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime p such that p ≡ 20 (mod 49).  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n = 0; returns 0 by convention */

    idx_t limit = 500;        /* initial sieve limit; will be doubled as needed */
    uint64_t *primes = NULL;  /* array of primes up to 'limit' */
    size_t cnt = 0;           /* number of primes returned by build_primes_upto */

    while (1) {
        build_primes_upto(limit, &primes, &cnt);

        idx_t found = 0;      /* how many qualifying primes we have seen so far */

        for (size_t i = 0; i < cnt; ++i) {
            if (primes[i] % 49 == 20) {
                ++found;
                if (found == n) {
                    seq_t result = primes[i];
                    free(primes);
                    return result;
                }
            }
        }

        free(primes);        /* deallocate primes array before enlarging the limit */
        limit *= 2;          /* enlarge the sieve range exponentially */
    }
}
