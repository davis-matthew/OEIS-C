/*
 * A067359  Real part of (5 + 12i)^n.
 * https://oeis.org/A067359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(n) = 10*a(n-1) - 169*a(n-2), a(1)=1, a(2)=5
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache: 1‑indexed array storing computed values */
    static seq_t *cache = NULL;  /* cache[1]..cache[cache_len] */
    static idx_t cache_len = 0;  /* highest index stored */

    /* fast return if already computed */
    if (n <= cache_len && cache != NULL) {
        return cache[n];
    }

    /* ensure cache has at least two elements for the base cases */
    if (cache_len < 2) {
        /* allocate 3 slots: we ignore cache[0] to keep 1‑based indexing */
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (cache == NULL) {
            /* memory allocation failed; return zero as a safe fallback */
            return (seq_t)0;
        }
        cache_len = 2;
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)5;   /* a(2) */
    }

    /* extend the cache up to n */
    if (n > cache_len) {
        /* reallocate to accommodate up to index n (1‑based) */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) {
            /* realloc failed, leave cache unchanged */
            return cache[n]; /* may be out of bounds but caller should not rely on this */
        }
        cache = new_cache;

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* recurrence: a(i) = 10*a(i-1) - 169*a(i-2) */
            cache[i] = (seq_t)10 * cache[i - 1] - (seq_t)169 * cache[i - 2];
        }
        cache_len = n;
    }

    return cache[n];
}
