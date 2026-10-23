/*
 * A036428  Square octagonal numbers.
 * https://oeis.org/A036428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   a(2) = 225
   a(n) = 194*a(n-1) - a(n-2) + 32   for n >= 3
   Uses a static cache so that calls may come in any order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n <= 0 */
        return 0;

    /* static cache with 1‑based indexing (cache[i-1] == a(i)) */
    static seq_t *cache        = NULL;
    static size_t cache_cap    = 0;   /* allocated slots (>= computed) */
    static idx_t  computed     = 0;   /* number of terms stored in cache */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;                    /* need at least two slots */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)              /* allocation failed – abort */
            return 0;
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)225; /* a(2) */
        computed = 2;
    }

    /* grow cache if the requested index is beyond current capacity */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap <<= 1;              /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                      /* realloc failed – abort */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return value if already known */
    if (n <= computed)
        return cache[n - 1];

    /* compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 2]; /* a(i-1) */
        seq_t prev2 = cache[i - 3]; /* a(i-2) */
        cache[i - 1] = (prev1 * (seq_t)194) - prev2 + (seq_t)32;
    }
    computed = n;
    return cache[n - 1];
}
