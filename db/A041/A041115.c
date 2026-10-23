/*
 * A041115  Denominators of continued fraction convergents to sqrt(66).
 * https://oeis.org/A041115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=8, for n≥2: a(n) = 16*a(n-1) + a(n-2) if n is even
 *            a(n) = 8*a(n-1) + a(n-2) if n is odd
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached sequence values */
    static size_t cache_cap = 0;    /* allocated capacity */
    static size_t cache_len = 0;    /* initialized entries */

    /* Ensure capacity for at least n+1 elements.  Double capacity each time. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base values if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)8;
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t val;
        if ((i & 1) == 0)            /* even i */
            val = (seq_t)16 * prev + prev2;
        else                         /* odd i */
            val = (seq_t)8 * prev + prev2;
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
