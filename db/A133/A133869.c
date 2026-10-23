/*
 * A133869  Numbers k such that 32*k + 1 is prime.
 * https://oeis.org/A133869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 542, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A133869: k such that 32*k + 1 is prime.  a(1) is the first such k. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* static cache for already found k‑values */
static seq_t *cache      = NULL;   /* array of seq_t */
static size_t cache_size = 0;      /* number of stored values */
static size_t cache_cap  = 0;      /* allocated capacity (in seq_t units) */
static idx_t  last_k     = 0;      /* last integer k that was tested */

/* ensure that the cache has at least 'need' entries.  The size is always
   doubled when increased to keep amortised O(1) growth. */
static void ensure_cache_cap(size_t need) {
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : need;
    seq_t *new_cache = seq_alloc_array(new_cap);
    for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    if (cache) seq_free_array(cache);
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term (1‑based) of OEIS A133869. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but guard */

    if (cache_size < (size_t)n) {
        ensure_cache_cap((size_t)n);
        while (cache_size < (size_t)n) {
            ++last_k;                                    /* candidate k */

            /* p = 32*k + 1 */
            seq_t k = (seq_t)last_k;
            seq_t p = ((seq_t)32 * k) + (seq_t)1;

            if (is_prime((idx_t)p))                      /* check primality */
                cache[cache_size++] = k;
        }
    }

    return cache[(size_t)n - 1];
}
