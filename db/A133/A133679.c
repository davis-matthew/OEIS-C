/*
 * A133679  a(n) = 7*a(n-1) + 56*a(n-2) for n>=3, a(0)=1, a(1)=7, a(2)=98.
 * https://oeis.org/A133679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*a(n-1) + 56*a(n-2), a(0)=1, a(1)=7, a(2)=98
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static idx_t cache_size = 0;     /* number of elements stored in cache */

/* Return the n-th term of the sequence A133679. */
seq_t a(idx_t n) {
    /* Ensure the cache is large enough. */
    if (n + 1 > cache_size) {
        /* Determine new size. */
        idx_t new_size = (n + 1 > 3 ? n + 1 : 3);

        /* Allocate or grow the cache. */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (cache == NULL) abort();   /* allocation failure */
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (tmp == NULL) abort();   /* allocation failure */
            cache = tmp;
        }

        /* Initialise the first three terms if the cache was empty. */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)7;
            cache[2] = (seq_t)98;
        }

        /* Compute new terms using the recurrence. */
        for (idx_t i = (cache_size > 2 ? cache_size : 3); i < new_size; ++i) {
            cache[i] = (seq_t)7 * cache[i - 1] + (seq_t)56 * cache[i - 2];
        }

        cache_size = new_size;
    }
    return cache[n];
}
