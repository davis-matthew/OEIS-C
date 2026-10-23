/*
 * A116558  a(n) = 6*a(n-4) - a(n-8).
 * https://oeis.org/A116558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 6*a(n-4) - a(n-8), a(0)=0, a(1)=1, a(2)=1, a(3)=5,
 *            a(4)=2, a(5)=5, a(6)=5, a(7)=29
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 50, u64 n <= 102,
 *            u128 n <= 202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recursive cache for the sequence */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_size = 0;    /* allocated size of cache   */
static idx_t max_computed = 0;   /* highest index currently computed */

/* Ensure the cache can hold indices up to n (inclusive).
   Allocate or grow the array as necessary. */
static void
ensure_cache_capacity(idx_t n)
{
    if ((size_t) n + 1 > cache_size) {
        size_t new_size = (size_t) n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort (no error string needed) */
            abort();
        }
        cache = tmp;
        cache_size = new_size;
    }
}

/* The required sequence function */
seq_t a(idx_t n)
{
    /* Base values for indices 0..7 */
    static const seq_t init[8] = {0, 1, 1, 5, 2, 5, 5, 29};

    /* First call: initialise the cache with the eight base terms. */
    if (cache == NULL) {
        size_t need = (size_t) n + 1;
        if (need < 8) need = 8;          /* at least keep indices 0..7 */
        cache = (seq_t *)calloc(need, sizeof(seq_t));
        if (!cache) abort();             /* allocation failure */

        /* Copy the base terms into the cache */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];

        cache_size = need;
        max_computed = 7;
    }

    /* If the requested index is within the already computed range,
       simply return it. */
    if (n <= max_computed)
        return cache[n];

    /* Extend the cache to include the new index if necessary. */
    ensure_cache_capacity(n);

    /* Compute missing terms up to n. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        cache[k] = 6 * cache[k - 4] - cache[k - 8];
    }
    max_computed = n;

    return cache[n];
}
