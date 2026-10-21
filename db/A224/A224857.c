/*
 * A224857  Numbers n such that 90n + 41 and 90n + 43 are twin primes.
 * https://oeis.org/A224857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer k such that 90k+41 and 90k+43 are twin
 *            primes. The sequence is generated lazily and cached
 * Range:     u8 n <= 45, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
/* Provided primality test */
#include "../utils/is_prime.h"

/* Static cache of discovered terms */
static seq_t *cache     = NULL;   /* array of terms */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* capacity of the array */

/* Candidate to be tested next */
static idx_t next_candidate = 0;

/* Ensure the cache can hold at least `need` terms. */
static void
ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* critical failure */

    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }

    cache     = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; undefined for n=0 */
        return 0;

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n) {
        idx_t cand = next_candidate;
        /* Compute the two candidate primes: 90*k + 41 and 90*k + 43. */
        idx_t p = 90 * cand + 41;
        idx_t q = p + 2;

        if (is_prime(p) && is_prime(q)) {
            /* Store the candidate. */
            ensure_cache(cache_len + 1);
            cache[cache_len] = (seq_t)cand;
            cache_len++;
        }

        next_candidate++;
    }

    return cache[n - 1];
}
