/*
 * A166400  Multiples of 23 whose digit reversal - 1 is also a multiple of 23.
 * https://oeis.org/A166400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 20, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../utils/reverse_decimal.h"

/* Computes the n-th term of OEIS A166400 (multiples of 23 whose digit reversal minus 1 is also a multiple of 23),
   using a simple brute‑force search with caching. */

static seq_t *cache = NULL;          /* cached terms */
static size_t cache_cap = 0;         /* capacity of cache array */
static idx_t cache_size = 0;         /* number of cached terms */
static seq_t next_multiple = 23;     /* next multiple of 23 to test */

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Lazily initialise the cache. */
    if (!cache) {
        cache_cap = 64;                      /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        assert(cache);
    }

    /* Generate terms until we have at least n terms cached. */
    while (cache_size < n) {
        seq_t m = next_multiple;                 /* current multiple of 23 */

        /* rev(m) - 1 must also be a multiple of 23. */
        seq_t revm = reverse_decimal(m);
        if ((revm - 1) % 23 == 0) {
            /* Expand the cache if needed. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                assert(new_cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = m;             /* store the term */
        }

        next_multiple += 23;                    /* advance to next candidate */
    }

    return cache[(size_t)(n - 1)];              /* 1‑based index */
}
