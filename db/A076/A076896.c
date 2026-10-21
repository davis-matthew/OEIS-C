/*
 * A076896  a(1) = 1, a(n) = n-a(floor(2n/3)).
 * https://oeis.org/A076896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = n - a(floor(2n/3)) for n ≥ 2
 * Range:     u8 n <= 426, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc, calloc, exit */

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array storing a(0..n) */
static idx_t cache_capacity = 0;     /* allocated size of cache array */
static idx_t cache_len = 0;          /* highest index that has been computed */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Expand allocation if necessary */
    if (!cache || n + 1 > cache_capacity) {
        idx_t new_cap = (cache_capacity > 0) ? cache_capacity : 1;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                 /* abort on allocation failure */
        /* zero any newly allocated part */
        if (new_cap > cache_capacity)
            for (idx_t i = cache_capacity; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute any missing terms in order */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;
        } else {
            idx_t j = (i * 2) / 3;                /* floor(2*i/3) */
            seq_t aj = cache[j];                  /* j < i, so already computed */
            cache[i] = (seq_t)i - aj;
        }
    }
    cache_len = n;   /* now all indices up to n are valid */
}

/* Public function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not defined for n=0, return 0 as guard */
    ensure_cache(n);
    return cache[n];
}
