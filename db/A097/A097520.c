/*
 * A097520  Numbers of the form p^11 + 11^p for p prime.
 * https://oeis.org/A097520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n^11 + 11^p_n (p_n is the n-th prime)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-01)
 * Helpers:   build_primes_upto, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

seq_t a(idx_t n)
{
    /* (1) Prime cache ---------------------------------------------- */
    /* We cache the list of primes so that repeated calls are fast.
       The cache is grown only when a larger n is requested.  */
    static uint64_t *prime_cache = NULL;
    static size_t prime_count = 0;     /* number of primes in the cache */

    if (n == 0)  /* offset is 1; n < 1 is irrelevant */
        return (seq_t)0;

    if (prime_count < (size_t)n) {
        /* Estimate an upper bound for the n-th prime.  */
        idx_t limit = _prime_limit_for_idx(n) + 100;   /* safety margin */

        uint64_t *new_cache = NULL;
        size_t new_count = 0;

        /* Build the list of primes up to 'limit'.  */
        build_primes_upto(limit, &new_cache, &new_count);

        /* Replace the old cache if we had one.  */
        if (prime_cache != NULL)
            free(prime_cache);

        prime_cache = new_cache;
        prime_count = new_count;
    }

    /* (2) Retrieve the n-th prime (1‑based). ------------------------ */
    /* The array is 0‑based, so the n‑th prime is at index n-1.  */
    seq_t p = (seq_t)prime_cache[(size_t)(n - 1)];

    /* (3) Compute p^11 and 11^p. ----------------------------------- */
    seq_t term_p = ipow(p, (seq_t)11);
    seq_t term_11 = ipow((seq_t)11, p);

    /* (4) Return the sum. ------------------------------------------ */
    return term_p + term_11;
}
