/*
 * A234312  Number of tilings of a 5 X n rectangle using n pentominoes of
 *          shapes L, X.
 * https://oeis.org/A234312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(a(n-2)+a(n-5)) for n>4 a(0)=1, a(1)=0, a(2)=2, a(3)=0,
 *            a(4)=4
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 50, u64 n <= 98, u128 n <= 195,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to handle arbitrary call order */
    static seq_t *cache   = NULL; /* pointer to the cache */
    static size_t cache_cap = 0;  /* current number of cached entries */

    /* grow cache if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        size_t old_cap = cache_cap;

        /* realloc with an explicit cast for C++ compatibility */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – return zero */

        /* initialise any new entries */
        for (size_t i = old_cap; i < new_cap; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)0;
            else if (i == 2)
                cache[i] = (seq_t)2;
            else if (i == 3)
                cache[i] = (seq_t)0;
            else if (i == 4)
                cache[i] = (seq_t)4;
            else
                /* recurrence: a(i) = 2*(a(i-2)+a(i-5)) */
                cache[i] = (seq_t)2 * (cache[i-2] + cache[i-5]);
        }
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
