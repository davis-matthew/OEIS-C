/*
 * A045313  Primes congruent to {1, 2, 3, 5} (mod 7).
 * https://oeis.org/A045313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime that is congruent to 1, 2, 3, or 5 (mod 7) */

#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        /* undefined for n < 1, return 0 */
        return 0;
    }

    /* Rough upper bound: about (7/4) * n * log(n) for large n;
       use a smaller constant for very small n */
    idx_t limit = (n < 6) ? 20 : n * 20;
    idx_t *primes = NULL;
    size_t cnt = 0;

    while (1) {
        /* Generate all primes ≤ limit.  build_primes allocates an array
           of ascending primes and stores its size in *cnt. */
        build_primes(limit, &primes, &cnt);

        idx_t pos = 0;
        for (size_t i = 0; i < cnt; ++i) {
            idx_t p = primes[i];
            idx_t r = p % 7;
            if (r == 1 || r == 2 || r == 3 || r == 5) {
                ++pos;
                if (pos == n) {
                    seq_t result = (seq_t)p;
                    free(primes);
                    return result;
                }
            }
        }

        /* Not enough primes, try a larger range */
        free(primes);
        limit *= 2;
    }
}
