/*
 * A142262  Primes congruent to 13 mod 43.
 * https://oeis.org/A142262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142262: primes p such that p ≡ 13 (mod 43)
   (sieve search: generate primes up to a growing limit until n terms
   in the desired residue class are found). */

#include <stdlib.h>
#include <stdbool.h>              /* for bool used by helpers */
#include "../utils/build_primes.h"/* provides void build_primes(idx_t limit, idx_t **out, size_t *cnt); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: a(1) = 13, callers should use n ≥ 1 */

    /* Start with a limit that typically contains enough primes for the density
       1/43 of all primes.  The factor 2000 is generous enough for n <= 10000. */
    idx_t limit = n * 2000U;
    if (limit < 10000U) limit = 10000U;

    while (1) {
        idx_t *primes    = NULL;
        size_t primes_cnt = 0;
        build_primes(limit, &primes, &primes_cnt);

        idx_t count = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            if ((primes[i] % 43U) == 13U) {
                ++count;
                if (count == n) {
                    seq_t result = (seq_t)primes[i];
                    free(primes);
                    return result;
                }
            }
        }
        free(primes);

        /* Not enough terms – double the limit and try again. */
        if (limit > SIZE_MAX / 2)
            limit = SIZE_MAX;
        else
            limit <<= 1;         /* multiply by 2 */
    }
}
