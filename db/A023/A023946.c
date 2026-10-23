/*
 * A023946  Expansion of g.f. 1/((1-x)*(1-5*x)*(1-10*x)*(1-11*x)).
 * https://oeis.org/A023946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 27*a(n-1) - 241*a(n-2) + 765*a(n-3) - 550*a(n-4),
 * a(0) = 1, a(1) = 27, a(2) = 488, a(3) = 7434 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* initialise cache with first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)1;      /* a(0) */
        cache[1] = (seq_t)27;     /* a(1) */
        cache[2] = (seq_t)488;    /* a(2) */
        cache[3] = (seq_t)7434;   /* a(3) */
        cache_len = 4;
    }

    /* compute missing terms using the linear recurrence */
    if (n >= cache_len) {
        /* enlarge capacity until it can hold index n */
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute forward from cache_len to n inclusive */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];

            seq_t val = ((seq_t)27) * a1
                        - ((seq_t)241) * a2
                        + ((seq_t)765) * a3
                        - ((seq_t)550) * a4;
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
