/*
 * A074041  Product of first n single (i.e., non-twin) primes.
 * https://oeis.org/A074041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n) = a(n‑1) * p_n where p_n is the n‑th single
 *            (non‑twin) prime
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   is_prime, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function caches results so that calls with non‑monotonic n are handled efficiently. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc.h"   /* provides seq_alloc(size_t) */

static seq_t *cache = NULL;    /* 1‑based cumulative products */
static size_t cache_cap = 0;   /* allocated array size (in seq_t’s) */
static idx_t cached_n = 0;     /* largest n computed so far */
static uint64_t next_candidate = 3;  /* odd candidate to test for single prime */

/* Return the n‑th term of A074041. */
extern seq_t a(idx_t n)
{
    /* Undefined below offset – return 0 to satisfy any caller. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Grow the cache to accommodate index n (1‑based). */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (new_cap < (size_t)(n + 1)) {
            new_cap *= 2;
        }
        seq_t *new_cache = seq_alloc(new_cap);
        if (!new_cache) {
            /* Allocation failure – abort */
            return (seq_t)0;
        }
        /* Preserve already computed values. */
        if (cache) {
            for (size_t i = 0; i <= (size_t)cached_n; ++i) {
                new_cache[i] = cache[i];
            }
            free((void *)cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values up to the requested index n. */
    while (cached_n < n) {
        ++cached_n;
        if (cached_n == 1) {
            /* The first single prime is 2. */
            cache[1] = (seq_t)2;
            continue;
        }

        /* Find the next single (non‑twin) prime. */
        uint64_t p = 0;
        while (true) {
            if (is_prime((idx_t)next_candidate)) {
                bool single = true;
                /* Twin test: prime must not have ±2 as primes. */
                if (next_candidate > 2) {
                    if (is_prime((idx_t)(next_candidate - 2))) single = false;
                    if (is_prime((idx_t)(next_candidate + 2))) single = false;
                }
                if (single) {
                    p = next_candidate;
                    /* Advance to the next odd number for future searches. */
                    next_candidate += 2;
                    break;
                }
            }
            next_candidate += 2;  /* skip even numbers */
        }

        /* Cumulative product: a(n) = a(n‑1) * p. */
        cache[cached_n] = cache[cached_n - 1] * (seq_t)p;
    }

    return cache[n];
}
