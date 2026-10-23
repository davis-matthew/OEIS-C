/*
 * A163084  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   a(1) = 13
   a(2) = 156
   a(3) = 1872
   a(4) = 22386   (computed from the numerator of the G.f.)
   a(n) = 11*a(n-1) + 11*a(n-2) + 11*a(n-3) - 66*a(n-4)  for n >= 5
   (Derived from (1-11t-11t^2-11t^3+66t^4) * A(t) = 1+2t+2t^2+2t^3+t^4) */

#include <stdlib.h>
#include <string.h>

/* static cache for already computed terms */
static seq_t *cache = NULL;       /* array of computed a(n) values      */
static idx_t cache_size = 0;      /* allocated size of the cache array  */
static idx_t computed_upto = 0;   /* highest index that has been computed */

/* ensure that the cache holds values up to index n; grows the array if
   necessary and computes missing terms using the recurrence. */
static void ensure_cache(idx_t n)
{
    /* If the cache hasn't been allocated yet, allocate it big enough
       for the requested index plus a few extra slots for the base
       values.  The memory is zeroed; base values will be filled below. */
    if (cache == NULL) {
        cache_size = (n + 1 > 5) ? (n + 1) : 5;
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        if (cache == NULL) return;   /* allocation failed */

        /* initialise the first few known terms */
        cache[0] = 1u;
        if (cache_size > 1) cache[1] = 13u;
        if (cache_size > 2) cache[2] = 156u;
        if (cache_size > 3) cache[3] = 1872u;
        if (cache_size > 4) cache[4] = 22386u;

        computed_upto = 4;
    }
    /* Grow the cache array if the requested index is out of bounds. */
    else if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;
        seq_t *new_cache = (seq_t *)calloc((size_t)new_size, sizeof(seq_t));
        if (new_cache == NULL) return;   /* allocation failed */
        memcpy(new_cache, cache, (size_t)cache_size * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing terms, starting just beyond the last computed
       index.  Only indices up to and including n are filled. */
    idx_t start = computed_upto + 1;
    for (idx_t i = start; i <= n; ++i) {
        if (i == 4) {
            /* a(4) already set during initialisation; skip recomputation. */
            continue;
        }
        /* For n >= 5 use the homogeneous recurrence. */
        seq_t term = (seq_t)11 * cache[i - 1]
                   + (seq_t)11 * cache[i - 2]
                   + (seq_t)11 * cache[i - 3]
                   - (seq_t)66 * cache[i - 4];
        cache[i] = term;
    }
    computed_upto = (n > computed_upto) ? n : computed_upto;
}

/* Public function: returns a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
