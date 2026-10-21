/*
 * A071784  Determinant of the n X n matrix whose element (i,j) equals the
 *          floor( Phi^(i-j) + 1).
 * https://oeis.org/A071784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n‑1) + a(n‑2) - 2*a(n‑3) + a(n‑4) (with a(1)=2, a(2)=2,
 *            a(3)=1, a(4)=−1). The OEIS generating function is
 *            (-3x²+2)/[(1−x)(1−x²+x³)], giving this 4‑term linear recurrence.
 *            This function computes a(n) for any n ≥ 1 using memoisation and
 *            dynamic growth of a static cache
 * Range:     i8 n <= 27, i16 n <= 47, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] stores a(i), i ≥ 1; index 0 unused */
    static seq_t *cache      = NULL;   /* pointer to cache array          */
    static idx_t cache_len   = 0;      /* number of valid entries (=size) */
    static idx_t cache_cap   = 0;      /* allocated capacity             */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* start with capacity for indices 0..4 (5 elements) */
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* out of memory: fall back */
        cache[0] = 0;                       /* unused placeholder            */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)-1;
        cache_len = 5;                       /* indices 0 .. 4 are valid       */
    }

    /* if requested n is already cached, return immediately */
    if (n < cache_len)
        return cache[(size_t)n];

    /* grow cache capacity if necessary */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 5;
        while (new_cap <= n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;  /* out of memory: fall back */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute terms up to and including a(n) */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val = cache[(size_t)(k - 1)]          /* a(k‑1) */
                 + cache[(size_t)(k - 2)]          /* a(k‑2) */
                 - (seq_t)2 * cache[(size_t)(k - 3)] /* -2*a(k‑3) */
                 + cache[(size_t)(k - 4)];          /* +a(k‑4)  */
        cache[(size_t)k] = val;
    }
    cache_len = n + 1;     /* now a(0)…a(n) are valid */

    return cache[(size_t)n];
}
