/*
 * A141767  A positive integer k is included if (p-1)*(p+1) divides k for
 *          every prime p that divides k.
 * https://oeis.org/A141767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that for every prime p
 *            dividing k, (p-1)*(p+1) divides k
 * Range:     u8 n <= 6, u16 n <= 253, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_len = 0;     /* number of values cached */
static size_t cache_cap = 0;     /* capacity of the cache array */
static uint64_t next_candidate = 2; /* next integer to test (after 1) */

/* Test whether a given integer k satisfies the property */
static bool satisfies(uint64_t k)
{
    if (k == 1)
        return true;

    /* Temporary arrays for factorization.
       64 is more than enough for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 fills primes, exps and cnt. */
    factor_u64(k, primes, exps, &cnt);

    seq_t k_seq = (seq_t)k;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        seq_t d = (seq_t)(p - 1) * (p + 1);   /* (p-1)*(p+1) */

        if (k_seq % d != 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 1 */
    if (n == 0)
        return 0;   /* undefined, but keeps function defined */

    /* initialise cache if needed */
    if (cache == NULL) {
        cache_cap = 64;                /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;                  /* a(1) */
        cache_len = 1;
    }

    /* if requested term already cached, return it */
    if ((size_t)n <= cache_len)
        return cache[(size_t)(n - 1)];

    /* generate terms until we reach n */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;   /* candidate integer */

        if (satisfies(cand)) {
            seq_t val = (seq_t)cand;

            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = val;
        }
    }

    return cache[(size_t)(n - 1)];
}
