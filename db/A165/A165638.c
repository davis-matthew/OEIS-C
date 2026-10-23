/*
 * A165638  a(n)=(6/7)*(2+5*(-6)^(n-1)).
 * https://oeis.org/A165638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = 6, a(n) = -6*a(n-1) + 12 for n >= 2
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 12, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache that keeps all already computed terms. */
    static seq_t *cache = NULL;         /* array of computed a(i) */
    static idx_t last_computed = 0;     /* highest index stored */
    static size_t cache_allocated = 0;  /* allocated size of cache   */

    /* Initialise cache with a(0) = 1 if not done yet. */
    if (cache == NULL) {
        cache_allocated = 1;
        cache = (seq_t *)malloc(cache_allocated * sizeof(seq_t));
        if (!cache) { return 0; }        /* kernel fallback; not expected */
        cache[0] = 1;
        last_computed = 0;
    }

    /* If the required term is already cached, return it immediately. */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Grow the cache to at least n‑th element. Allocate more wisely */
    /* to avoid repeated reallocations for large n. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_allocated) {
        size_t new_alloc = cache_allocated ? cache_allocated : 64;
        while (new_alloc < needed)
            new_alloc <<= 1;           /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) { return 0; }       /* allocation failed */
        cache = tmp;
        cache_allocated = new_alloc;
    }

    /* Compute and store missing terms in order. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next = -6 * prev + 12;   /* recurrence: a(n) = -6*a(n-1)+12 */
        cache[i] = next;
    }
    last_computed = n;

    return cache[n];
}
