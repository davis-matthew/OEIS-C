/*
 * A219998  Prime-free decades such that the previous decade is also
 *          prime-free.
 * https://oeis.org/A219998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219998:  Prime‑free decades such that the previous decade is
   also prime‑free.
   Formula (definition): a(n) is the nth integer n (n≥1) such that
   10n–9, 10n–7, 10n–3, 10n–1, 10n+1, 10n+3, 10n+7, 10n+9 are all composite.
   The implementation enumerates n in increasing order and caches the
   qualifying values so that subsequent calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_cap = 0;         /* capacity of cache */
static size_t cache_len = 0;         /* number of values stored */
static idx_t candidate = 1;          /* next candidate n to test */

/* Ensure the cache has room for at least new_cap elements. */
static void ensure_cache_cap(size_t new_cap) {
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap * 2 : 16;
    if (cap < new_cap) cap = new_cap;
    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failure – abort (runtime error). */
        abort();
    }
    cache = new_cache;
    cache_cap = cap;
}

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n=0 – return 0 */

    /* Extend the cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        /* Evaluate the current candidate candidate. */
        uint64_t base = 10ULL * (uint64_t)candidate;
        static const int offsets[8] = {9, 7, 3, 1, -1, -3, -7, -9};
        bool ok = true;
        for (int i = 0; i < 8 && ok; ++i) {
            int64_t temp = (int64_t)base + offsets[i];
            if (temp < 0) { ok = false; break; }
            uint64_t val = (uint64_t)temp;
            /* Use the provided primality test for uint64 values. */
            if (is_prime((idx_t)val)) ok = false;
        }

        if (ok) {
            /* Store in cache. */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return cache[n - 1];
}
