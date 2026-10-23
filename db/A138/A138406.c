/*
 * A138406  a(n) = prime(n)^5 - prime(n)^3.
 * https://oeis.org/A138406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^5 - prime(n)^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"

/* Computes the n-th term of OEIS sequence A138406. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* Undefined for 0-index */

    /* static cache of primes */
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;

    /* Ensure we have at least the first n primes in the cache. */
    if (primes == NULL || primes_cnt < n) {
        /* Estimate an upper bound for the nth prime. */
        idx_t limit = _prime_limit_for_idx(n);
        /* Rebuild the prime list up to the new limit. */
        free(primes);                     /* free old cache if any */
        build_primes(limit, &primes, &primes_cnt);
    }

    /* Retrieve the n-th prime (1‑based). */
    uint32_t p = (uint32_t)primes[n - 1];

    /* Perform the computation in seq_t to preserve type rules. */
    seq_t p_t = (seq_t)p;
    seq_t p2 = p_t * p_t;
    seq_t p3 = p2 * p_t;
    seq_t p5 = p3 * p2;   /* p^5 = p^3 * p^2 */

    return p5 - p3;        /* prime(n)^5 - prime(n)^3 */
}
