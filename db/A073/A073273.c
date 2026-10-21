/*
 * A073273  a(n) = floor(sqrt(prime(n)*prime(n+2))).
 * https://oeis.org/A073273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(prime(n) * prime(n+2)))
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 3492, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"

/* Computes a(n) for n ≥ 1.  The sequence is defined for any
 * n ≥ 1, and the function is safe for any order of calls. */
seq_t a(idx_t n)
{
    /* Negative or zero indices are outside the offset; return 0. */
    if (n < 1) return 0;

    /* Keep a static cache of primes so that repeated calls reuse the
     * generated table.  The cache is discarded and regenerated
     * only when the requested index is larger than the current size. */
    static uint64_t *prime_cache = NULL;
    static size_t prime_cnt   = 0;     /* number of entries in prime_cache */

    /* Ensure we have at least n+2 primes (to access prime(n) and prime(n+2)). */
    if (prime_cnt < (size_t)(n + 2)) {
        /* Release old cache if any. */
        free(prime_cache);

        /* Estimate an upper bound on the required prime value. */
        idx_t limit = _prime_limit_for_idx(n + 2);

        /* Rebuild the prime table.  The helper allocates memory
         * for the array; we hold on to it until a larger request
         * necessitates a rebuild. */
        build_primes_upto(limit, &prime_cache, &prime_cnt);
    }

    /* Obtain the needed prime numbers.  prime_cache is 0‑based indexing:
     * prime_cache[0] = prime(1).  Thus prime(n)      = prime_cache[n-1]
     * and prime(n+2) = prime_cache[n+1]. */
    seq_t p1 = (seq_t)prime_cache[n - 1];
    seq_t p2 = (seq_t)prime_cache[n + 1];

    /* Compute floor(sqrt(p1 * p2)).  The product may exceed 64‑bit, so
     * use the arbitrary‑precision helper seq_mul.  The root is taken
     * with floor_sqrt, which operates on seq_t. */
    seq_t product = seq_mul(p1, p2);
    return floor_sqrt(product);
}
