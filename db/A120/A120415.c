/*
 * A120415  Expansion of 1/(1-x-x^3-x^6).
 * https://oeis.org/A120415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-6); * Initial values: a(0)=1,
 *            a(1)=1, a(2)=1, * a(3)=2, a(4)=3, a(5)=4
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 53, u64 n <= 104,
 *            u128 n <= 207, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache    = NULL;   /* array of computed terms          */
    static idx_t  cache_len = 0;    /* next index that is not yet filled */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* --- initialization -------------------------------------------- */
    if (cache == NULL) {
        /* at least capacity for 6 terms (needed for recurrence) */
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)                       /* allocation failed */
            return (seq_t)0;

        /* seed the first six terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)4;

        /* we now have terms 0..5 ready */
        cache_len = 6;
    }

    /* --- request beyond known range --------------------------------- */
    if (n + 1 > cache_cap) {
        /* increase capacity (doubling strategy) */
        size_t new_cap = cache_cap * 2;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                       /* allocation failed */
            return (seq_t)0;
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* compute any missing terms */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence valid only for i >= 6 */
            cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 6];
        }
        cache_len = n + 1;             /* now we have terms 0..n */
    }

    return cache[n];
}
