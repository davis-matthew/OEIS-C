/*
 * A172172  Sums of NW-SE diagonals of triangle A172171.
 * https://oeis.org/A172172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 9; a(0)=0; a(1)=1
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;     /* cached values */
    static idx_t  cache_len = 0;      /* number of cached elements (last index + 1) */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* allocate initial storage if needed */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory, return 0 as a safe fallback */
    }

    /* grow the cache if it does not hold up to n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else {
            cache[i] = cache[i-1] + cache[i-2] + (seq_t)9;
        }
    }
    cache_len = n + 1;

    return cache[n];
}
