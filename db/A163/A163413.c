/*
 * A163413  a(n) = 14*a(n-1) - 47*a(n-2) for n > 1; a(0) = 1, a(1) = 11.
 * https://oeis.org/A163413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 14*a(n-1) - 47*a(n-2); a(0) = 1, a(1) = 11
 */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache: values a(0)..a(max_computed) */
    static seq_t *cache        = NULL; /* allocated array */
    static idx_t cache_cap    = 0;     /* current size of cache */
    static idx_t max_computed = 0;     /* highest n already computed */

    /* ---------- first call: allocate initial cache ------------------ */
    if (cache == NULL) {
        /* at least two slots for a(0) and a(1) */
        cache_cap = (n >= 1 ? n + 1 : 2);
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL) { /* allocation failed – abort with zero */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1; /* a(0) */
        cache[1] = (seq_t)11; /* a(1) */
        max_computed = 1;     /* we know a(0) and a(1) */
    }

    /* ---------- expand cache if necessary ---------------------------- */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 2;      /* double */
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* realloc failed – keep old cache */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---------- compute missing terms -------------------------------- */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)14 * cache[i - 1] - (seq_t)47 * cache[i - 2];
        }
        max_computed = n;
    }

    /* ---------- return the requested value --------------------------- */
    return cache[n];
}
