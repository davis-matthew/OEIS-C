/*
 * A042762  Numerators of continued fraction convergents to sqrt(912).
 * https://oeis.org/A042762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 302*a(n-2) - a(n-4), a(0) = 30, a(1) = 151, a(2) = 9090,
 *            a(3) = 45601
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer holding computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of valid entries          */
    static idx_t cache_cap = 0;    /* allocated capacity (at least cache_len) */

    /* initialise cache with the first four terms */
    if (cache_len == 0)
    {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)30;
        cache[1] = (seq_t)151;
        cache[2] = (seq_t)9090;
        cache[3] = (seq_t)45601;
        cache_len = 4;                      /* valid indices 0..3 */
    }

    /* expand cache if needed (capacity) and compute new values up to n */
    if (n >= cache_len)
    {
        /* grow capacity until it fits n+1 elements */
        if (cache_cap <= n)
        {
            idx_t new_cap = cache_cap;
            if (new_cap == 0) new_cap = 4;
            while (new_cap <= n) new_cap <<= 1;      /* double size */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;        /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            cache[i] = cache[i-2] * (seq_t)302 - cache[i-4];
        }
        cache_len = n + 1;    /* now indices 0..n are valid */
    }

    return cache[n];
}
