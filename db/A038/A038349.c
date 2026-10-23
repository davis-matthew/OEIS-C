/*
 * A038349  Partial sums of primes congruent to 1 mod 6.
 * https://oeis.org/A038349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of primes congruent to 1 mod 6 (OEIS A038349)
   a(n) = a(n-1) + p_n   where p_n is the n‑th prime ≡1 (mod 6),
   a(1) = 7.  The function caches the list of such primes and
   computes the sum on each call. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* declares build_primes_upto */

static uint64_t *prime_cache     = NULL;   /* primes ≡1 (mod6) */
static size_t    prime_cache_len = 0;      /* number of cached primes */
static idx_t     sieve_limit     = 0;       /* current sieve upper bound */
static bool      cache_valid     = false;  /* true when prime_cache is filled */

/* Ensure that prime_cache contains at least n primes ≡1 (mod6). */
static void ensure_primes(size_t n)
{
    if (cache_valid && prime_cache_len >= n)
        return;                               /* nothing to do */

    /* Start with a reasonable initial limit. */
    if (!cache_valid || sieve_limit == 0)
        sieve_limit = 1000;

    /* Grow the sieve limit until we have enough primes. */
    while (1) {
        uint64_t *tmp = NULL;
        size_t   cnt  = 0;
        build_primes_upto(sieve_limit, &tmp, &cnt);
        if (tmp == NULL)           /* allocation failure – give up */
            return;

        /* Count primes ≡1 (mod6) among the new block. */
        size_t need = 0;
        for (size_t i = 0; i < cnt; ++i)
            if (tmp[i] % 6ULL == 1ULL)
                ++need;

        /* Allocate a new cache large enough for all collected primes. */
        uint64_t *new_cache = NULL;
        if (prime_cache != NULL) {
            new_cache = (uint64_t *)realloc(prime_cache,
                                            (prime_cache_len + need) * sizeof(uint64_t));
        } else {
            new_cache = (uint64_t *)malloc((prime_cache_len + need) * sizeof(uint64_t));
        }

        if (new_cache == NULL) {   /* allocation failure – clean up */
            free(tmp);
            free(prime_cache);
            prime_cache = NULL;
            prime_cache_len = 0;
            cache_valid = false;
            return;
        }

        /* Copy over existing cached primes and the new ones that satisfy the
           congruence condition. */
        size_t idx = 0;
        if (prime_cache != NULL) {
            /* Existing primes already present in new_cache since realloc/malloc retains data. */
            idx = prime_cache_len;
        }

        for (size_t i = 0; i < cnt && idx < prime_cache_len + need; ++i)
            if (tmp[i] % 6ULL == 1ULL) {
                new_cache[idx++] = tmp[i];
            }

        free(tmp);
        free(prime_cache);          /* free old cache if any */
        prime_cache = new_cache;
        prime_cache_len = idx;

        if (prime_cache_len >= n) { /* we have enough primes */
            cache_valid = true;
            break;
        }

        /* Not enough – double the limit and try again. */
        sieve_limit *= 2;
    }
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset starts at 1 – a(0) is undefined but return 0 */
        return (seq_t)0;

    /* Ensure we have at least n qualifying primes cached. */
    ensure_primes((size_t)n);

    /* Compute the partial sum from the cached primes. */
    seq_t sum = 0;
    for (size_t i = 0; i < (size_t)n; ++i)
        sum += (seq_t)prime_cache[i];

    return sum;
}
