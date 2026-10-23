/*
 * A042524  Numerators of continued fraction convergents to sqrt(791).
 * https://oeis.org/A042524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 450*a(n-2) - a(n-4) a(0)=28, a(1)=225, a(2)=12628,
 *            a(3)=101249
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the nth term of A042524. */
seq_t a(idx_t n)
{
    /* static cache and its current allocated size */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cache_size = 0; /* number of entries in cache (>=0) */

    /* Ensure the cache can hold all entries up to n. */
    if (cache == NULL) {
        /* Allocate initial space (just in case n < 4). */
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        cache_size = n + 1;
        if (cache == NULL) return (seq_t)0; /* allocation failed */
    } else if (cache_size <= (size_t)n) {
        size_t new_size = (n + 1) > 2 * cache_size ? (n + 1) : 2 * cache_size;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Fill the cache up to n. */
    static const seq_t init_terms[4] = {28, 225, 12628, 101249};

    for (size_t i = 0; i <= (size_t)n; ++i) {
        if (i < 4) {
            if (cache[i] != init_terms[i]) cache[i] = init_terms[i];
        } else {
            /* The recurrence uses terms that have already been computed. */
            seq_t term = 450 * cache[i - 2] - cache[i - 4];
            if (cache[i] != term) cache[i] = term;
        }
    }

    return cache[(size_t)n];
}
