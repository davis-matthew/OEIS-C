/*
 * A077926  Expansion of (1-x)^(-1)/(1+2*x+x^3).
 * https://oeis.org/A077926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-2) - a(n-3) + a(n-4); Initial values:
 *            a(0)=1, a(1)=-1, a(2)=3, a(3)=-6
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 27, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Initialise the cache with the first four terms. */
static void init_cache(void) {
    if (cache != NULL) return;
    cache_size = 4;
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    if (!cache) exit(1);          /* abort if memory allocation fails */
    cache[0] = 1;
    cache[1] = -1;
    cache[2] = 3;
    cache[3] = -6;
}

/* Ensure the cache contains values up to index `upto`. */
static void ensure_cache(idx_t upto) {
    if (upto < (idx_t)cache_size) return;  /* already computed */

    if (cache == NULL)            /* first call */
        init_cache();

    while ((idx_t)cache_size <= upto) {
        seq_t next =
            -cache[cache_size-1] +          /* -a(n-1) */
              2*cache[cache_size-2] -       /* +2*a(n-2) */
              cache[cache_size-3] +         /* -a(n-3) */
              cache[cache_size-4];          /* +a(n-4) */

        size_t new_size = cache_size + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);          /* abort on failure */
        cache = tmp;
        cache[cache_size] = next;
        cache_size = new_size;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
