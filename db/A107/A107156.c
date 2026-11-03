/*
 * A107156  Primes of the form 2x^2 + 25y^2.
 * https://oeis.org/A107156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 550, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 8083 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A107156: primes of the form 2x² + 25y²
   Implemented by incremental prime search and direct
   representation test. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

/* Check whether a prime p can be written as 2x² + 25y². */
static bool has_representation(uint64_t p)
{
    /* p = 2 for the first term, handled separately elsewhere. */
    /* Search over possible y. */
    uint64_t y_max = (uint64_t)isqrt((idx_t)(p / 25ULL));
    for (uint64_t y = 0; y <= y_max; ++y) {
        uint64_t rem = p - 25ULL * y * y;   /* p – 25y² */
        if (rem & 1ULL) continue;           /* must be even */
        uint64_t half = rem >> 1;           /* (p – 25y²) / 2 */
        uint64_t x = (uint64_t)isqrt((idx_t)half);
        if (x * x == half) return true;     /* found a representation */
    }
    return false;
}

/* Return the nth prime of the form 2x² + 25y² (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static idx_t cache_cap = 0, cache_len = 0;
    static uint64_t next_candidate = 3;      /* start with the first odd prime */

    /* One‑time allocation for the cache. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = seq_malloc(cache_cap);
    }

    /* Ensure the first term (prime 2) is in the cache. */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    /* Generate further terms until we have at least n of them. */
    while (cache_len < n) {
        /* Only odd candidates are needed (except 2 which is already handled). */
        uint64_t p = next_candidate;
        if (is_prime_u64(p)) {
            if (has_representation(p)) {
                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    idx_t new_cap = cache_cap * 2;
                    seq_t *tmp = seq_realloc(cache, new_cap);
                    if (!tmp) return 0;   /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)p;
            }
        }
        next_candidate += 2;                 /* skip even numbers */
    }

    return cache[n - 1];                     /* 1‑based indexing */
}
