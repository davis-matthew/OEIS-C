/*
 * A162326  Let a(0) = a(1) = 1, and n*a(n) = 2*(-7+5*n)*a(n-1) +
 *          9*(2-n)*a(n-2) for n >= 2.
 * https://oeis.org/A162326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 + 2*∑_{m=1}^{n-1} a(m)*a(n-m)
 * with a(0) = 1, a(1) = 1
 */
#include <stddef.h>
#include <stdlib.h>

#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;     /* dynamically allocated cache */
static size_t cache_cap = 0;    /* allocated size of cache */
static idx_t  computed  = 0;    /* number of values already computed (0…computed‑1) */

/* enlarge cache to hold at least (n+1) entries */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap >= needed) return;

    /* new capacity: next power of two, at least 16 */
    size_t new_cap = 16;
    while (new_cap < needed) new_cap <<= 1;

    /* allocate new array */
    seq_t *new_cache = seq_alloc_new_array(new_cap);
    if (!new_cache) abort();          /* out of memory */

    /* copy old values */
    for (idx_t i = 0; i < computed; ++i) new_cache[i] = cache[i];

    /* free old array if it existed */
    if (cache) free_seq_array(cache);

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* sequence generator */
seq_t a(idx_t n)
{
    /* initialise to base values lazily */
    if (computed == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        computed = 2;
    }

    if (n < computed) return cache[n];

    /* compute missing values up to n */
    grow_cache(n);
    for (idx_t k = computed; k <= n; ++k) {
        seq_t sum = 0;
        for (idx_t m = 1; m < k; ++m) {
            sum += cache[m] * cache[k - m];
        }
        cache[k] = 1 + 2 * sum;
    }
    computed = n + 1;
    return cache[n];
}
