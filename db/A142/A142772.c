/*
 * A142772  Primes congruent to 45 mod 59.
 * https://oeis.org/A142772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 45 mod 59 – search and cache implementation */

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */

static seq_t *cache = NULL;   /* cached primes */
static size_t cap   = 0;      /* capacity of cache */
static size_t len   = 0;      /* number of stored primes */
static uint64_t last = 0;     /* last prime examined */
static bool init   = false;  /* initialized flag */

/* Return the n‑th prime p such that p ≡ 45 (mod 59), 1‑based indexing */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!init) {
        cap   = 32;
        cache = (seq_t *)malloc(cap * sizeof(*cache));
        assert(cache != NULL);
        init = true;
    }

    /* extend the cache until it contains at least n desired primes */
    while (len < n) {
        /* find the next prime larger than 'last' */
        last = next_prime((uint64_t)last);

        /* accept the prime only if it satisfies the congruence condition */
        if ((last % 59ULL) == 45ULL) {
            /* grow the cache if necessary */
            if (len == cap) {
                size_t newcap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(*cache));
                assert(tmp != NULL);
                cache = tmp;
                cap   = newcap;
            }
            cache[len++] = (seq_t)last;
        }
    }

    /* return the requested term (1‑based index) */
    return cache[n - 1];
}
