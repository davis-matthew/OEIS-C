/*
 * A079406  Recamán variation: a(1)=1, then a(n)=a(n-1)-n if n is already in
 *          the sequence, a(n)=a(n-1)+n otherwise.
 * https://oeis.org/A079406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = a(n–1) – n if n already occurs in
 *            the sequence, a(n) = a(n–1) + n otherwise
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[0] unused, cache[1] = a(1) = 1 */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    size_t i, j;

    /* initialise the cache on first call */
    if (cache_size == 0) {
        cache_cap  = 16;
        cache      = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out‑of‑memory guard */
        cache[0] = (seq_t)0;                 /* unused index 0 */
        cache[1] = (seq_t)1;                 /* a(1) */
        cache_size = 2;                      /* elements stored so far */
    }

    /* grow the cache until we have a(n) computed */
    while (cache_size <= (size_t)n) {
        seq_t prev   = cache[cache_size - 1];
        seq_t target = (seq_t)cache_size;      /* the new index value */

        /* scan existing values to see if the new index already appears */
        int found = 0;
        for (j = 1; j < cache_size; ++j) {
            if (cache[j] == target) { found = 1; break; }
        }

        /* compute the next term */
        seq_t next = found ? prev - target : prev + target;

        /* grow the array if full */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* out‑of‑memory guard */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* store the new term and advance */
        cache[cache_size++] = next;
    }

    return cache[n];
}
