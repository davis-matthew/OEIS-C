/*
 * A143760  a(n+1) = a(n)^2 + n^2, a(1) = 1.
 * https://oeis.org/A143760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^2 + n^2, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of stored terms */
    static size_t cap   = 0;   /* allocated capacity */

    /* Initialise cache with a(1) = 1 */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure fallback */
        cap = 1;
        cache[0] = (seq_t)1; /* a(1) */
        len  = 1;
    }

    /* Return already computed value */
    if (n <= len)
        return cache[n - 1];

    /* Compute terms up to a(n) iteratively */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 2];         /* a(i-1) */
        seq_t idx  = (seq_t)(i - 1);       /* i-1 as seq_t */
        seq_t val  = prev * prev + idx * idx;

        /* Resize cache if needed */
        if (len >= cap) {
            size_t new_cap = (cap == 0) ? 1 : cap * 2;
            seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (new_arr == NULL) return (seq_t)0; /* allocation failure fallback */
            cache = new_arr;
            cap   = new_cap;
        }

        cache[len] = val;
        ++len;
    }

    return cache[n - 1];
}
