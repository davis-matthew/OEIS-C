/*
 * A121801  Expansion of 2*x^2*(3-x)/((1+x)*(1-3*x+x^2)).
 * https://oeis.org/A121801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3), a(1) = 0, a(2) = 6, a(3) =
 *            10 (Recurrence given in OEIS entry A121801)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy memoisation. Keep all terms computed so far. */
    static idx_t cache_count = 0;
    static seq_t *cache = NULL;

    if (n < 1)   /* undefined – not expected, but safe */ return 0;

    /* Initialise the cache with the first three terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 3);
        /* a(1), a(2), a(3) */
        cache[0] = 0;
        cache[1] = 6;
        cache[2] = 10;
        cache_count = 3;
    }

    /* Extend the cache up to the requested index */
    if (n > cache_count) {
        /* Resize to exactly the needed size (simple and efficient enough) */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * n);
        if (tmp == NULL)  /* allocation failed – abort with default value */
            return 0;
        cache = tmp;
        /* Compute new terms using the recurrence */
        for (idx_t k = cache_count + 1; k <= n; ++k) {
            /* k is the 1‑based index of the term to compute */
            seq_t val = 2 * cache[k - 2] + 2 * cache[k - 3] - cache[k - 4];
            cache[cache_count] = val;
            ++cache_count;
        }
    }

    return cache[n - 1];  /* cache index 0 corresponds to a(1) */
}
