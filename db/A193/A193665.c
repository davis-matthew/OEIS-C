/*
 * A193665  Q-residue of A075392, where Q=A075392. (See Comments.)
 * https://oeis.org/A193665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)+(-n-4)*a(n-1)+(n+1)*a(n-2)-a(n-3)=0, with initial values
 *            a(0)=1, a(1)=6, a(2)=33
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the recurrence (a(n) = (n+4)*a(n-1) - (n+1)*a(n-2) + a(n-3))
   and memoises previously computed terms in a static cache. */ 

#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *cache = NULL;          /* cache for computed terms */
static size_t cache_cap = 0;         /* current capacity of cache */
static idx_t  computed_to = 0;       /* largest n for which value is cached */

/* Compute a(n) for any non‑negative n, in any order of calls. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        size_t init_cap = 3;                     /* enough for a(0), a(1), a(2) */
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;                      /* out of memory: give 0 */

        for (size_t i = 0; i < init_cap; ++i) cache[i] = 0;
        cache_cap       = init_cap;
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 33;
        computed_to   = 2;
    }

    /* Array already contains required term */
    if (n <= computed_to) return cache[(size_t)n];

    /* Ensure the cache can hold up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {    /* realloc failed */
            free(cache);
            cache = NULL;
            cache_cap = 0;
            return 0;
        }
        /* Initialise newly allocated slots to zero */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = computed_to + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)(i + 4) * cache[(size_t)(i - 1)];
        seq_t term2 = (seq_t)(i + 1) * cache[(size_t)(i - 2)];
        cache[(size_t)i] = term1 - term2 + cache[(size_t)(i - 3)];
    }

    computed_to = n;
    return cache[(size_t)n];
}
