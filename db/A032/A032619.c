/*
 * A032619  Numbers k such that k concatenated with k+4 is a prime.
 * https://oeis.org/A032619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 3026, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, isPrimeGeneric, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032619: numbers k such that concatenation of k with k+4 is prime. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions (provided by the build system) */
#include "../utils/isPrimeGeneric.h"
#include "../utils/concat_parts.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* allocated capacity of the cache */

/* The smallest candidate k not yet examined */
static idx_t next_k = 1;

/* Ensure the cache has at least 'needed' slots.  Reallocate if necessary. */
static void ensure_cache_cap(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2U : 16U;
    if (needed > new_cap)
        new_cap = needed;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (tmp == NULL) {
        /* In the unlikely event of a memory allocation failure we abort. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;               /* offset is 1 – returning 0 for n==0 is harmless */

    /* If the request is within the already cached range, return it immediately. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1U];

    /* Extend the cache until we have computed the n‑th term. */
    while (cache_len < n) {
        idx_t k = next_k;
        while (true) {
            seq_t concat = concat_parts(k, k + 4);
            if (isPrimeGeneric(concat)) {
                /* Append k to the cache. */
                ensure_cache_cap(cache_len + 1);
                cache[cache_len] = (seq_t)k;
                cache_len++;

                /* The next candidate will be the following integer. */
                next_k = k + 1;
                break;           /* proceed to the next term */
            }
            ++k;                /* try the next integer */
        }
    }

    return cache[(size_t)n - 1U];
}
