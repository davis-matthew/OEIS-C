/*
 * A176372  y-values in the solution to x^2 - 66*y^2 = 1.
 * https://oeis.org/A176372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176372: y-values in the solution to x^2 - 66*y^2 = 1.
   Recurrence: a(1) = 0, a(2) = 8, a(n) = 130*a(n-1) - a(n-2) for n ≥ 3. */

#include <stdlib.h>
#include <stddef.h>   /* for NULL */

static seq_t *cache = NULL;         /* 1‑based array of previously computed terms */
static idx_t cache_size = 0;        /* largest index currently stored (≥ 2 after init) */
static idx_t cache_cap  = 0;        /* allocated capacity (≥ cache_size+1) */

/* Ensure the cache has space for at least 'min_cap' elements. */
static void cache_ensure(idx_t min_cap) {
    if (cache_cap >= min_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < min_cap) new_cap <<= 1;          /* double until sufficient */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                               /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (1‑indexed). */
seq_t a(idx_t n) {
    /* Initialise cache with first two terms on first call. */
    if (!cache) {
        cache_ensure(8);
        cache[1] = (seq_t)0;    /* a(1) */
        cache[2] = (seq_t)8;    /* a(2) */
        cache_size = 2;
    }

    /* Direct returns for the first two terms. */
    if (n == 1) return cache[1];
    if (n == 2) return cache[2];

    /* Compute missing terms up to n. */
    if (n > cache_size) {
        cache_ensure(n + 1);    /* ensure indices up to n exist */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)130 * cache[i - 1] - cache[i - 2];
        }
        cache_size = n;
    }

    return cache[n];
}
