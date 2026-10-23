/*
 * A190733  Expansion of (4*x+2)/(1+sqrt(1-4*x-4*x^2)).
 * https://oeis.org/A190733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)*a(n-1) + 14*(n-2)*a(n-2) + 20*(n-3)*a(n-3) +
 *            8*(n-4)*a(n-4)) / (n+1) with initial values a(0)=1, a(1)=3,
 *            a(2)=5, a(3)=15
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (number of elements) */
    static idx_t cache_len = 0;   /* number of initialized terms */

    /* Ensure capacity at least n+1 */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap < n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);               /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values once */
    if (cache_len == 0) {
        /* Need to hold the first four values even if n < 4 */
        if (cache_cap < 4) {
            idx_t need = 4;
            idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
            while (new_cap < need)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)15;
        cache_len = 4;
    }

    /* If the requested term is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* The recurrence is only valid for i >= 4; for 0..3 we already have values */
        if (i < 4)
            continue;

#ifdef __cplusplus
        /* C++: use seq_t arithmetic (handles arbitrary‑precision types) */
        seq_t numer = (seq_t)(i - 1) * cache[i - 1];
        numer += (seq_t)14 * (seq_t)(i - 2) * cache[i - 2];
        numer += (seq_t)20 * (seq_t)(i - 3) * cache[i - 3];
        numer += (seq_t)8  * (seq_t)(i - 4) * cache[i - 4];
        cache[i] = numer / (i + 1);
#else
        /* C: use 128‑bit intermediate to avoid overflow of 64‑bit types */
        __int128 numer128 = 0;
        numer128 += (__int128)(i - 1) * cache[i - 1];
        numer128 += (__int128)14 * (i - 2) * cache[i - 2];
        numer128 += (__int128)20 * (i - 3) * cache[i - 3];
        numer128 += (__int128)8  * (i - 4) * cache[i - 4];
        cache[i] = (seq_t)(numer128 / (i + 1));
#endif
    }
    cache_len = n + 1;

    return cache[n];
}
