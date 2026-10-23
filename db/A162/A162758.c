/*
 * A162758  a(n) = 14*a(n-1)-46*a(n-2) for n > 1; a(0) = 1, a(1) = 8.
 * https://oeis.org/A162758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1)-46*a(n-2) for n > 1; a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"


static seq_t *cache = NULL;
static idx_t cache_cap = 0;    /* allocated size */
static idx_t max_computed = 0; /* highest index with a value already stored */

/* Make sure the cache has room for indices up to n */
static void ensure_cache_size(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    idx_t new_cap = n + 1;
    seq_t *new_cache = seq_alloc_array((size_t)new_cap);
    if (cache != NULL) {
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise base values on first use */
static void initialise_base(idx_t n)
{
    if (cache == NULL) {
        idx_t cap = (n >= 1) ? n + 1 : 2;   /* at least space for a(0) and a(1) */
        cache = seq_alloc_array((size_t)cap);
        cache[0] = (seq_t)1;
        if (cap > 1)
            cache[1] = (seq_t)8;
        cache_cap = cap;
        max_computed = (cap > 1) ? 1 : 0;
    }
}

seq_t a(idx_t n)
{
    initialise_base(n);
    ensure_cache_size(n);

    if (n <= max_computed)
        return cache[n];

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t1 = (seq_t)14 * cache[i - 1];
        seq_t t2 = (seq_t)46 * cache[i - 2];
        cache[i] = t1 - t2;    /* recurrence */
    }
    max_computed = n;
    return cache[n];
}
