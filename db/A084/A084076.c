/*
 * A084076  Length of list created by n substitutions k -> Range[-1-abs(k),
 *          abs(k)+1] starting with {1}.
 * https://oeis.org/A084076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((7*n-2)(7*n-1)a(n-1) +
 *            4(2*n-1)(7*n+2)a(n-2))/((n+2)(7*n-5)), a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size */
    static idx_t last  = 0;       /* highest index computed */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)1;                 /* a(0) */
        if (n >= 1) cache[1] = (seq_t)5;      /* a(1) */
        last = (n >= 1) ? 1 : 0;
    }

    /* extend cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        /* initialise newly allocated slots */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {

        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)5;
        } else {
#if defined(__cplusplus)
/* ----- C++ (arbitrary–precision) path ----- */
            seq_t ti = (seq_t)i;
            seq_t u = (ti * 7 - 2) * (ti * 7 - 1);
            seq_t v = 4 * (ti * 2 - 1) * (ti * 7 + 2);
            seq_t w = (ti + 2) * (ti * 7 - 5);
            cache[i] = (u * cache[i-1] + v * cache[i-2]) / w;
#else
/* ----- C (integral) path using 128‑bit intermediates ----- */
            unsigned __int128 ui = (unsigned __int128)7 * i;
            unsigned __int128 u = (ui - 2) * (ui - 1);
            unsigned __int128 v = 4 * ((unsigned __int128)2 * i - 1) * (ui + 2);
            unsigned __int128 w = ((unsigned __int128)i + 2) *
                                  ((unsigned __int128)7 * i - 5);
            unsigned __int128 num = u * (unsigned __int128)cache[i-1] +
                                    v * (unsigned __int128)cache[i-2];
            cache[i] = (seq_t)(num / w);
#endif
        }
    }

    last = n;
    return cache[n];
}
