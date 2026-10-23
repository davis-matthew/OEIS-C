/*
 * A173534  a(n)=a(n-1)+2*a(n-2)-[a(n-1)/2]-[a(n-4)/2]-[a(n-5)/2].
 * https://oeis.org/A173534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=8 a(n) = a(n-1) + 2*a(n-2)
 *            - floor(a(n-1)/2) - floor(a(n-4)/2) - floor(a(n-5)/2) for n >= 5
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already-computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;
    static idx_t cache_len   = 0;

    /* initialise cache with the first five terms */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;            /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)8;
        cache_len = 5;
    }

    /* if requested value already known, return it */
    if (n < cache_len) return cache[n];

    /* grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!cache) return (seq_t)0;            /* allocation failed */
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a4 = (i >= 4) ? cache[i - 4] : (seq_t)0;  /* i >= 5 when used */
        seq_t a5 = (i >= 5) ? cache[i - 5] : (seq_t)0;

        /* recurrence */
        cache[i] = a1 + ((seq_t)2 * a2)
                        - (a1 / (seq_t)2)
                        - (a4 / (seq_t)2)
                        - (a5 / (seq_t)2);
    }
    cache_len = n + 1;
    return cache[n];
}
