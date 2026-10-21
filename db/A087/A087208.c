/*
 * A087208  Expansion of e.g.f. exp(x)/(1-x^2).
 * https://oeis.org/A087208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 is the recurrence a(n) = n*(n-1)*a(n-2) + 1,
 *            a(0) = 1, a(1) = 1. The sequence is A087208
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   alloc_array, free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_array.h"
#include "../utils/free_array.h"

#ifdef __cplusplus
    
    
#else
    
    
#endif

static seq_t *cache = NULL;     /* dynamic cache of computed values */
static idx_t cache_size = 0;   /* current size of the cache */

/* Ensure that the cache contains at least indices 0 … n.           */
/* Computes missing values using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    /* Allocate a new slightly larger array. */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = alloc_array(new_size);

    /* Copy old values first. */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* Initialize missing entries. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0 || i == 1) {
            new_cache[i] = (seq_t)1;
        } else {
            seq_t prod = (seq_t)i * ((seq_t)i - (seq_t)1);
            new_cache[i] = prod * new_cache[i - 2] + (seq_t)1;
        }
    }

    /* Replace old cache. */
    free_array(cache);
    cache     = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* First call: initialise cache for indices 0 and 1. */
    if (cache == NULL) {
        cache = alloc_array(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    ensure_cache(n);
    return cache[n];
}
