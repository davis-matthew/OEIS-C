/*
 * A028130  Expansion of 1/((1-4x)(1-6x)(1-7x)(1-8x)).
 * https://oeis.org/A028130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 56*a(n-2) - 2*4^n + 3*6^n (n>=2), a(0)=1,
 *            a(1)=25
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache and auxiliary state */
    static seq_t *cache = NULL;
    static idx_t cache_n = (idx_t)(-1);   /* last computed index */
    static idx_t cache_cap = 0;           /* allocated capacity */
    static seq_t pow4_cur = 0;            /* 4^i for current i */
    static seq_t pow6_cur = 0;            /* 6^i for current i */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate small initial capacity */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* memory error – unlikely in the test harness */

        /* base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache_n  = 1;

        /* powers for the next term to compute (i=2 will use 4^2 etc.) */
        pow4_cur = (seq_t)4;   /* 4^1 */
        pow6_cur = (seq_t)6;   /* 6^1 */
    }

    /* If the requested index is already cached, return directly */
    if (n <= cache_n)
        return cache[n];

    /* Ensure capacity for the new terms */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL)
            return 0; /* memory error */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute new terms sequentially */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        /* advance powers to i (pow4_cur and pow6_cur are currently 4^(i-1) etc.) */
        if (i > 1) {
            pow4_cur *= (seq_t)4;
            pow6_cur *= (seq_t)6;
        }

        /* recurrence: a(i) = 15*a(i-1) - 56*a(i-2) - 2*4^i + 3*6^i */
        seq_t a_im1 = cache[i - 1];
        seq_t a_im2 = cache[i - 2];
        seq_t term = 15 * a_im1
                   - 56 * a_im2
                   - 2 * pow4_cur
                   + 3 * pow6_cur;

        cache[i] = term;
    }
    cache_n = n;
    return cache[n];
}
