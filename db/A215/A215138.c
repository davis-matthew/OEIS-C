/*
 * A215138  Numbers k such that k > d(k)^2/2 where d = A000005.
 * https://oeis.org/A215138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A215138: Numbers k such that k > d(k)^2/2 where d = A000005.
   a(n) returns the nth such integer (1‑indexed).  The implementation
   builds a divisor‑count table on demand and memoises the satisfying
   integers.  No hard‑coded table is used. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute divisor counts up to N (inclusive).  Returns a malloced
   array of size N+1; element 0 is unused.  Caller must free it. */
static uint32_t *compute_divisor_counts(size_t N)
{
    uint32_t *d = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    if (!d) return NULL;                 /* allocation failure – highly unlikely */

    for (size_t i = 1; i <= N; ++i) {
        for (size_t j = i; j <= N; j += i) {
            ++d[j];
        }
    }
    return d;
}

/* Static cache state */
static seq_t *cache      = NULL;   /* array of found terms */
static size_t cache_len  = 0;      /* number of terms currently stored */
static size_t limit      = 0;      /* largest k for which divisor counts
                                    were computed */

/* Ensure the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    /* Grow the cache only if we do not yet hold enough terms. */
    while (cache_len < n) {
        /* Pick an initial sieve limit or double the previous one. */
        size_t new_limit = (limit == 0) ? 1024 : limit * 2;

        uint32_t *divs = compute_divisor_counts(new_limit);
        if (!divs) return;                 /* allocation failure – unlikely */

        /* Count how many numbers satisfy 2*k > d(k)^2 up to new_limit. */
        size_t count = 0;
        for (size_t k = 1; k <= new_limit; ++k) {
            uint64_t dk   = divs[k];
            uint64_t lhs  = 2ULL * (uint64_t)k;
            uint64_t rhs  = dk * dk;
            if (lhs > rhs) ++count;
        }

        if (count == 0) {   /* nothing satisfies the inequality – try larger limit */
            free(divs);
            limit = new_limit;
            continue;
        }

        /* Allocate cache for the newly found terms. */
        seq_t *new_cache = mem_new(seq_t, count);
        if (!new_cache) {
            free(divs);
            return;                      /* allocation failure – unlikely */
        }

        size_t pos = 0;
        for (size_t k = 1; k <= new_limit; ++k) {
            uint64_t dk   = divs[k];
            uint64_t lhs  = 2ULL * (uint64_t)k;
            uint64_t rhs  = dk * dk;
            if (lhs > rhs) {
                new_cache[pos++] = (seq_t)k;
            }
        }

        /* Replace old cache if this is the first fill; otherwise keep existing. */
        if (cache == NULL) {
            cache = new_cache;
            cache_len = count;
            limit = new_limit;
        } else {
            /* If we already had a cache, simply append the new terms. */
            seq_t *tmp = mem_resize(seq_t, cache, (cache_len + count));
            if (!tmp) {                    /* allocation failure – unlikely */
                mem_free(new_cache);
                free(divs);
                return;
            }
            /* Move new terms into the extended cache. */
            for (size_t i = 0; i < count; ++i) {
                tmp[cache_len + i] = new_cache[i];
            }
            mem_free(new_cache);
            cache = tmp;
            cache_len += count;
            if (new_limit > limit) limit = new_limit;
        }

        free(divs);
    }
}

/* Public function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* safe fallback; undefined for n=0 */
    ensure_cache(n);
    return cache[n - 1];
}
