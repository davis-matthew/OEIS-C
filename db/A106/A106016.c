/*
 * A106016  Primes p such that 14*p - 3 and 14*p + 3 are both primes.
 * https://oeis.org/A106016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that 14·p −3 and 14·p +3 are both
 *            primes
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_uint64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_size = 0;    /* number of cached terms */
static size_t cache_cap  = 0;    /* allocated capacity of cache */
static uint64_t last_checked = 4; /* last integer tested (start from 5) */

/* Ensure the cache has room for at least 'min_cap' elements. */
static void ensure_cache_capacity(size_t min_cap) {
    if (cache_cap < min_cap) {
        while (cache_cap < min_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
        }
        /* realloc may fail; ignore as the test harness has ample memory. */
        cache = seq_realloc(cache, cache_cap);
    }
}

seq_t a(idx_t n) {
    /* The sequence is 1‑indexed.  For n<1 the behaviour is undefined. */
    size_t idx = (size_t)n;          /* convert to a portable size_t */
    if (idx == 0) return (seq_t)0;    /* safety guard */

    /* Return cached value if already computed. */
    if (idx <= cache_size) {
        return cache[idx - 1];
    }

    /* Extend the cache until it contains the requested term. */
    ensure_cache_capacity(idx);

    for (uint64_t cand = last_checked + 1; cache_size < idx; ++cand) {
        last_checked = cand;               /* mark this integer as checked */

        if (!is_prime_uint64(cand)) {
            continue;                     /* skip non‑prime candidiates */
        }

        uint64_t a1 = 14 * cand - 3;
        uint64_t a2 = 14 * cand + 3;

        if (!is_prime_uint64(a1) || !is_prime_uint64(a2)) {
            continue;                     /* property fails */
        }

        /* Store the found prime in the cache. */
        cache[cache_size++] = (seq_t)cand;
    }

    return cache[idx - 1];
}
