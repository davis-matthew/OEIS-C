/*
 * A109173  Recursive form of A109845 but with a(1)=1.
 * https://oeis.org/A109173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=a(n-1)^2+(-1)^n*a(n-1)+(-1)^n (n≥1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>   /* for memcpy */

/* memoised recurrence for A109173 */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* 1‑based cache, cache[0] unused */
    static idx_t  cache_cap  = 0;      /* allocated number of elements   */
    static idx_t  cache_size = 0;      /* highest n computed so far     */

    /* ----- initialise cache on first call ----- */
    if (cache == NULL) {
        cache_cap = 16;                           /* arbitrary start size */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* allocation failure */
        cache[0]  = (seq_t)0;                     /* unused placeholder   */
        cache[1]  = (seq_t)1;                     /* a(1) = 1             */
        cache_size = 1;
    }

    /* ----- grow cache if needed ----- */
    if (n + 1 > cache_cap) {                     /* need room for a(n)   */
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;          /* allocation failure   */
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* ----- compute missing values up to n ----- */
    if (n > cache_size) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev   = cache[i - 1];
            seq_t square = prev * prev;            /* a(n-1)^2            */
            seq_t inc    = prev + (seq_t)1;        /* a(n-1)+1            */
            /* odd n : subtract inc; even n : add inc */
            if (i & (idx_t)1)
                cache[i] = square - inc;          /* odd n */
            else
                cache[i] = square + inc;          /* even n */
        }
        cache_size = n;
    }

    return cache[n];
}
