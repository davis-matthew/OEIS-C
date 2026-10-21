/*
 * A218540  Reduced third-order Patalan numbers.
 * https://oeis.org/A218540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=1; a(2)=1; a(3)=5; for n>=4: n*(n+2)*(n-1)*a(n) +
 *            (n-1)*(n-2)*(n+4)*a(n-1) - 3*(3*n-4)*(3*n-7)*(n+2)*a(n-2) -
 *            3*(3*n-10)*(n+4)*(3*n-7)*a(n-3) = 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation ------------------------------------- */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise base cases --------------------------------------------- */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 5;
        cache_len = 4;
    }

    /* return cached value if already known ------------------------------ */
    if (n < cache_len) return cache[n];

    /* grow cache to accommodate up to index n -------------------------- */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    /* compute missing terms --------------------------------------------- */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) { cache[i] = 1; continue; }
        if (i == 1) { cache[i] = 1; continue; }
        if (i == 2) { cache[i] = 1; continue; }
        if (i == 3) { cache[i] = 5; continue; }

#if defined(__SIZEOF_INT128__)
        /* use 128‑bit intermediate when available ---------------------- */
        __int128 t1 = (__int128)(i - 1) * (i - 2) * (i + 4)
                      * (__int128)cache[i - 1];
        __int128 t2 = (__int128)3 * (3 * i - 4) * (3 * i - 7) * (i + 2)
                      * (__int128)cache[i - 2];
        __int128 t3 = (__int128)3 * (3 * i - 10) * (i + 4) * (3 * i - 7)
                      * (__int128)cache[i - 3];

        __int128 numerator   = t2 + t3 - t1;
        __int128 denominator = (__int128)i * (i + 2) * (i - 1);
        cache[i] = (seq_t)(numerator / denominator);

#else
        /*  use seq_t as intermediate ----------------------------------- */
        seq_t t1 = cache[i - 1];
        t1 *= (seq_t)(i - 1);
        t1 *= (seq_t)(i - 2);
        t1 *= (seq_t)(i + 4);

        seq_t t2 = cache[i - 2];
        t2 *= (seq_t)3;
        t2 *= (seq_t)(3 * i - 4);
        t2 *= (seq_t)(3 * i - 7);
        t2 *= (seq_t)(i + 2);

        seq_t t3 = cache[i - 3];
        t3 *= (seq_t)3;
        t3 *= (seq_t)(3 * i - 10);
        t3 *= (seq_t)(i + 4);
        t3 *= (seq_t)(3 * i - 7);

        seq_t numerator   = t2 + t3;
        numerator -= t1;

        seq_t denominator = (seq_t)i;
        denominator *= (seq_t)(i + 2);
        denominator *= (seq_t)(i - 1);

        cache[i] = numerator / denominator;
#endif
    }

    /* update cache length and return result ---------------------------- */
    cache_len = new_len;
    return cache[n];
}
