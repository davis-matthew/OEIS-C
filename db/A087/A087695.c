/*
 * A087695  Numbers n such that n + 3 and n - 3 are both prime.
 * https://oeis.org/A087695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1745, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find numbers n such that n±3 are both prime. */
/* The function returns the nth term (n≥1) of OEIS A087695. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/build_sieve.h"

/* The types seq_t and idx_t are supplied by the build environment. */

static uint8_t *sieve = NULL;           /* sieve array: 1 = prime, 0 = composite   */
static idx_t   sieve_limit = 0;         /* current sieve upper bound (inclusive)   */
static seq_t  *cache  = NULL;           /* cached sequence values                 */
static size_t  cache_cap  = 0;          /* capacity of cache                      */
static idx_t   cached_terms = 0;        /* number of terms stored                */
static idx_t   last_checked  = 0;       /* last candidate processed              */

/* Extend the sieve to cover at least 'new_limit'. */
static void extend_sieve(idx_t new_limit)
{
    uint8_t *new_sieve = (uint8_t *)malloc((size_t)new_limit + 1);
    if (!new_sieve) exit(1);
    build_sieve(new_sieve, new_limit);
    free(sieve);
    sieve = new_sieve;
    sieve_limit = new_limit;
}

/* Ensure that the cache holds at least 'needed' terms. */
static void ensure_terms(idx_t needed)
{
    if (cached_terms >= needed) return;

    if (sieve_limit == 0)                 /* first call – initialise */
        extend_sieve(200000);             /* start with a modest sieve */

    /* start from the smallest even candidate that could be a term */
    idx_t cand = (last_checked < 8) ? 8 : last_checked + 2;

    while (cached_terms < needed) {
        /* make sure the sieve covers cand±3 */
        if (cand + 3 > sieve_limit) {
            idx_t new_limit = sieve_limit * 2;
            if (new_limit <= cand + 3) new_limit = cand + 3;
            extend_sieve(new_limit);
        }

        /* check the current candidate */
        if (sieve[cand - 3] && sieve[cand + 3]) {
            /* grow cache if necessary */
            if (cached_terms == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(1);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cached_terms++] = cand;
        }

        cand += 2;                      /* only even numbers need checking */
        last_checked = cand - 2;
    }
}

/* External function computing the nth term of A087695. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, return 0 */
    ensure_terms(n);
    return cache[n - 1];
}
