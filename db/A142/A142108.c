/*
 * A142108  Primes congruent to 25 mod 36.
 * https://oeis.org/A142108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 536, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A142108:
 *  a(n) = the nth prime congruent to 25 modulo 36   (offset 1)
 *
 *  The routine builds a cache of primes up to a dynamically chosen
 *  limit and returns the nth qualifying prime.  The cache is expanded
 *  only as needed, so calls with arbitrary ordering of n are fully
 *  supported.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* The helper that returns all primes ≤ limit.  Its header must be
 * included by the framework; we include it explicitly here. */
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Static state: cache of primes, their count, the current limit,
     * and the number of qualifying primes found so far. */
    static uint64_t *prime_cache      = NULL; /* array returned by build_primes_upto */
    static size_t   prime_cnt         = 0;    /* number of primes in the cache */
    static idx_t    current_limit     = 0;    /* limit up to which we have sieved */
    static size_t   qual_cnt          = 0;    /* number of primes ≡25 (mod 36) found */

    /* a(0) is undefined; return 0 instead of causing UB. */
    if (n < 1)
        return (seq_t)0;

    /* Initialize the first sweep if required. */
    if (prime_cache == NULL) {
        /* A generous upper bound: nth such prime is < 25*n for
         * reasonably large n (e.g., 10 000).  Ensure a minimal
         * starting point to avoid tiny sieves. */
        current_limit = (idx_t)(n * 250);
        if (current_limit < (idx_t)1000)
            current_limit = (idx_t)1000;
    }

    /* Expand the sieve until we have at least n qualifying primes. */
    for (;;) {
        /* Re-create the sieve when the cache is empty or we need more data. */
        if (prime_cache == NULL) {
            build_primes_upto(current_limit, &prime_cache, &prime_cnt);

            /* Count the qualifying primes in the new cache. */
            qual_cnt = 0;
            for (size_t i = 0; i < prime_cnt; ++i) {
                if ((prime_cache[i] % 36ULL) == 25ULL)
                    ++qual_cnt;
            }
        }

        /* If enough primes have been found, stop extending. */
        if (qual_cnt >= (size_t)n)
            break;

        /* Need to gather more primes: double the limit and resend. */
        current_limit *= 2;
        free(prime_cache);
        prime_cache = NULL;
        prime_cnt   = 0;
        qual_cnt    = 0;
    }

    /* Retrieve the nth qualifying prime from the cache. */
    size_t found = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if ((prime_cache[i] % 36ULL) == 25ULL) {
            ++found;
            if (found == (size_t)n)
                return (seq_t)prime_cache[i];
        }
    }

    /* This point should never be reached. */
    return (seq_t)0;
}
