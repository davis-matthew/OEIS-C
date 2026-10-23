/*
 * A041686  Numerators of continued fraction convergents to sqrt(363).
 * https://oeis.org/A041686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=19, a(1)=362, a(2)=13775, a(3)=262087; \
 * for n>=4: a(n)=724*a(n-2)-a(n-4) */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* helpers for allocating arrays that work in C and C++ */
static seq_t* _alloc_seq_array(idx_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}
static void _free_seq_array(seq_t *p) {
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

/* compute the nth term of A041686 */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;   /* number of terms stored, initially 0 */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* grow the array if necessary */
    if (n >= cache_cap) {
        idx_t needed = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = _alloc_seq_array(needed);
        for (idx_t i = 0; i < cache_len; ++i)  /* copy old values */
            new_cache[i] = cache[i];
        _free_seq_array(cache);
#else
        new_cache = (seq_t*)realloc(cache,
                                    needed * sizeof(seq_t));
        /* realloc keeps existing contents */
#endif
        cache     = new_cache;
        cache_cap = needed;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)          cache[i] = (seq_t)19;
        else if (i == 1)     cache[i] = (seq_t)362;
        else if (i == 2)     cache[i] = (seq_t)13775;
        else if (i == 3)     cache[i] = (seq_t)262087;
        else                 cache[i] = (seq_t)724 * cache[i-2] - cache[i-4];
    }

    cache_len = n + 1;
    return cache[n];
}
