/*
 * A145301  a(n) = 12*a(n-1) - 30*a(n-2) with a(0)=1 and a(1)=6.
 * https://oeis.org/A145301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 30*a(n-2); a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* seq_t and idx_t are supplied by the build */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0..cache_last] holds a(i) */
    static idx_t cache_cap = 0;      /* allocated size of cache array (index+1) */
    static idx_t cache_last = 0;     /* index of last computed element */

    /* initialise or enlarge cache if necessary */
    if (cache == NULL) {
        /* first allocation */
        cache_cap = (n >= 1 ? n + 1 : 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();

        cache[0] = (seq_t)1;
        cache_last = 0;
        if (n >= 1) {
            cache[1] = (seq_t)6;
            cache_last = 1;
        }
    } else {
        if (cache_cap <= n) {
            size_t new_cap = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        if (cache_last < 1 && n >= 1) {
            cache[1] = (seq_t)6;
            cache_last = 1;
        }
    }

    /* compute missing values up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = (seq_t)(12 * cache[i - 1] - 30 * cache[i - 2]);
    }
    cache_last = n;

    return cache[n];
}
