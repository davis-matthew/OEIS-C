/*
 * A179604  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + x)/(1 - 2*x - 9*x^2 - 2*x^3).
 * https://oeis.org/A179604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 9*a(n-2) + 2*a(n-3) Initial values: a(0) = 1,
 *            a(1) = 3, a(2) = 15
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored (>= cache_len) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initial seed values */
    if (cache == NULL) {
#ifndef __cplusplus
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#else
        cache_cap = 8;
        cache = new seq_t[cache_cap];
#endif
        if (cache == NULL) return 0; /* allocation failure, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)15;
        cache_len = 3; /* we have values 0..2 */
    }

    if (n < 3)
        return cache[n];

    /* grow cache until we reach n */
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap << 1;
#ifdef __cplusplus
            seq_t *new_cache = new seq_t[new_cap];
#else
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
            if (new_cache == NULL)
                return 0; /* allocation failure */
            /* copy existing elements */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
#ifndef __cplusplus
            free(cache);
#else
            delete[] cache;
#endif
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* recurrence: a(n) = 2*a(n-1) + 9*a(n-2) + 2*a(n-3) */
        seq_t val = (seq_t)2 * cache[cache_len - 1]
                  + (seq_t)9 * cache[cache_len - 2]
                  + (seq_t)2 * cache[cache_len - 3];
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n];
}
