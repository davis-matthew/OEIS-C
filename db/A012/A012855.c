/*
 * A012855  a(0) = 0, a(1) = 1, a(2) = 1; thereafter a(n) = 5*a(n-1) -
 *          4*a(n-2) + a(n-3).
 * https://oeis.org/A012855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + ...?
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Correct recurrence: a(0)=0, a(1)=1, a(2)=1; for n>=3 */
/* a(n) = 5*a(n-1) - 4*a(n-2) + a(n-3) */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache   = NULL;   /* array of computed values */
    static size_t cap     = 0;      /* allocated size of cache   */
    static size_t len     = 0;      /* number of values actually stored (len==cap after init) */

    /* initialise first three terms once */
    if (cache == NULL) {
        size_t init_cap = 3;
        if (init_cap == 0) init_cap = 1;
#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = (seq_t *)malloc(sizeof(seq_t) * init_cap);
#endif
        if (cache == NULL) return (seq_t)0; /* allocation failure – return 0 as fallback */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cap     = init_cap;
        len     = init_cap;
    }

    /* make sure we have space up to n */
    size_t n_s = (size_t)n;
    if (n_s + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 8;
        while (newcap <= n_s + 1) newcap <<= 1;   /* double until sufficient */

#ifdef __cplusplus
        seq_t *tmp = new seq_t[newcap];
        for (size_t i = 0; i < len; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (tmp == NULL) return (seq_t)0; /* reallocation failed */
        cache = tmp;
#endif
        cap = newcap;
    }

    /* compute any missing terms up to n */
    for (size_t i = len; i <= n_s; ++i) {
        /* recurrence valid for i>=3; for i=0,1,2 cache already contains values */
        if (i >= 3) {
            cache[i] = (seq_t)5 * cache[i - 1]
                     - (seq_t)4 * cache[i - 2]
                     +          cache[i - 3];
        }
    }
    if (n_s + 1 > len)
        len = n_s + 1;

    return cache[n_s];
}
