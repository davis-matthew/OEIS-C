/*
 * A142932  Primes congruent to 15 mod 64.
 * https://oeis.org/A142932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 207, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime ≡ 15 (mod 64) (OEIS A142932). */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    if (n == 0)              /* offset starts at 1 – n=0 is not defined */
        return (seq_t)0;

    /* Initial search bound; increase exponentially until we find n */
    idx_t limit = n * 64;
    if (limit < 128)
        limit = 128;

    while (1) {
        idx_t *primes = NULL;
        size_t count = 0;

        /* Generate all primes ≤ limit. */
        build_primes(limit, &primes, &count);

        idx_t found = 0;
        for (size_t i = 0; i < count; ++i) {
            if ((primes[i] & 63ULL) == 15ULL) { /* check residue 15 mod 64 */
                ++found;
                if (found == n) {
                    seq_t result = (seq_t)primes[i];
                    free(primes);
                    return result;
                }
            }
        }

        free(primes);
        /* Insufficient primes – double the search limit. */
        limit <<= 1;
    }
}
