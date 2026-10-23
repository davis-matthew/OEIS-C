/*
 * A022308  a(n) = a(n-1) + a(n-2) + 1 for n>1, a(0)=0, a(1)=3.
 * https://oeis.org/A022308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 1 for n>1, a(0) = 0, a(1) = 3
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache  = NULL;    /* array of terms */
    static size_t cap      = 0;     /* allocated size of cache   */
    static idx_t max_done = (idx_t)-1; /* highest index computed */

    /* first call – allocate space for indices 0 and 1 */
    if (cache == NULL) {
        /* Allocate at least two slots (indices 0 and 1) */
        cache = (seq_t *)malloc((size_t)2 * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure – return 0 as a safe fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        cap      = 2;
        max_done = 1;
    }

    /* If the requested index is already computed, return it */
    if (n <= max_done) {
        return cache[n];
    }

    /* Ensure the cache is large enough for indices up to n */
    if ((idx_t)cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failure – return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms incrementally */
    for (idx_t i = max_done + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)1;
    }
    max_done = n;

    return cache[n];
}
