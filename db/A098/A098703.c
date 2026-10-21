/*
 * A098703  a(n) = (3^n + phi^(n-1) + (-phi)^(1-n)) / 5, where phi denotes the
 *          golden ratio A001622.
 * https://oeis.org/A098703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098703:  a(n) = (3^n + φ^(n‑1) + (‑φ)^(1‑n)) / 5  (φ = golden ratio)
   Equivalent recurrence:
        a(0) = 0,  a(1) = 1,  a(2) = 2
        a(n) = 4*a(n‑1) – 2*a(n‑2) – 3*a(n‑3)   for n ≥ 3
   The implementation uses memoised recurrence and grows the cache
   as needed.  It works for both C11 and C++17 builds.               */

#include <stdlib.h>

/* compute the nth term of A098703 */
seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for 0 ≤ i ≤ last_cached */
    static seq_t *cache = NULL;
    static idx_t last_cached = (idx_t)-1;   /* -1 indicates not initialised */

    /* initialise cache for the three seed values */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (cache == NULL)  /* allocation failure – unlikely but safe */
            return 0;
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        last_cached = 2;
    }

    /* if requested value is already cached, return it */
    if (n <= last_cached)
        return cache[n];

    /* ensure enough space in the cache */
    /* current allocated size */
    static size_t cache_size = 3;
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size < (size_t)(n + 1))
            new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)   /* allocation failure – unlikely but safe */
            return 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* compute terms up to n using the recurrence */
    for (idx_t i = last_cached + 1; i <= n; ++i) {
        seq_t v = 4 * cache[i - 1];
        v -= 2 * cache[i - 2];
        v -= 3 * cache[i - 3];
        cache[i] = v;
    }
    last_cached = n;
    return cache[n];
}
