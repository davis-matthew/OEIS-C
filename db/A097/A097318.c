/*
 * A097318  Numbers with more than one prime factor and, in the ordered
 *          factorization, the exponent never increases when read from left to
 *          right.
 * https://oeis.org/A097318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 9594, u32 n <= 9594, u64 n <= 9594,
 *            u128 n <= 9594, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097318:
   Numbers whose ordered factorisation has at least two prime factors
   and whose exponents are non‑increasing (e1 >= e2 >= ... >= em).
   This implementation searches integers sequentially, storing
   the discovered terms in a static cache. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Helper to build a list of primes up to a given limit. */
#include "../utils/build_primes_upto.h"
/* Integer square root for idx_t types. */
#include "../utils/int_sqrt.h"

/* Ensure we have enough primes for factorisation. */
static void ensure_primes(idx_t limit, uint64_t **primes, size_t *cnt)
{
    static idx_t cur_limit = 0;
    static uint64_t *cur_primes = NULL;
    static size_t cur_cnt = 0;

    if (limit <= cur_limit)
        return;                     /* already sufficient */

    if (cur_primes)
        free(cur_primes);           /* discard old list */

    build_primes_upto(limit, &cur_primes, &cur_cnt);

    *primes = cur_primes;
    *cnt    = cur_cnt;
    cur_limit = limit;
}

/* Return true if x satisfies the sequence condition. */
static bool good_number(idx_t x, uint64_t *primes, size_t primes_cnt)
{
    if (x < 6)               /* smallest admissible number */
        return false;

    idx_t remaining = x;
    int  distinct   = 0;                /* number of distinct primes seen */
    int  last_exp   = -1;               /* exponent of previous prime */

    for (size_t i = 0; i < primes_cnt &&
                       (idx_t)primes[i] * (idx_t)primes[i] <= remaining;
         ++i) {
        uint64_t p = primes[i];
        if (remaining % p == 0) {
            int exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            ++distinct;
            if (distinct == 1) {
                last_exp = exp;
            } else {
                if (exp > last_exp)
                    return false;          /* exponents would increase */
                last_exp = exp;
            }
        }
    }

    if (remaining > 1) {                /* remaining prime factor > sqrt(x) */
        ++distinct;
        int exp = 1;
        if (distinct == 1) {
            last_exp = exp;
        } else {
            if (exp > last_exp)
                return false;
            last_exp = exp;
        }
    }

    return distinct >= 2;                /* need at least two distinct primes */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for index 0 */

    /* Static state shared across all calls. */
    static seq_t  *cache     = NULL;
    static size_t cache_cap = 0;        /* current allocated capacity */
    static size_t cache_len = 0;        /* number of terms stored */

    static uint64_t *prime_list = NULL;
    static size_t   prime_cnt   = 0;

    static idx_t next_candidate = 2;    /* integer to test next */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Ensure we have primes up to sqrt(next_candidate). */
        idx_t needed = int_sqrt(next_candidate) + 1;
        ensure_primes(needed, &prime_list, &prime_cnt);

        /* Check the current candidate. */
        if (good_number(next_candidate, prime_list, prime_cnt)) {
            /* Expand cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_cache)               /* out of memory */
                    abort();
                if (cache) {
                    memcpy(new_cache, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache     = new_cache;
                cache_cap = new_cap;
            }
            /* Store the found term. */
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;                 /* try the next integer */
    }

    return cache[n - 1];
}
