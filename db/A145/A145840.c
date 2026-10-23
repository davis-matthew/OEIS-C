/*
 * A145840  Number of 4-compositions of n.
 * https://oeis.org/A145840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 8*a(n+3) - 12*a(n+2) + 8*a(n+1) - 2*a(n) Initial
 *            values: a(0)=1, a(1)=4, a(2)=26, a(3)=164, a(4)=1031
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed values up to the maximum
       index requested so far.  The cache is grown only as needed. */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0; /* number of values stored */
    static size_t cache_cap = 0; /* allocated capacity */

    /* initialise cache with the first five values */
    if (cache_len == 0) {
        cache_cap = 16;                       /* arbitrary initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0; /* allocation failure – unlikely in tests */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)26;
        cache[3] = (seq_t)164;
        cache[4] = (seq_t)1031;
        cache_len = 5;
    }

    /* grow cache if required to hold up to index n */
    if (n >= cache_len) {
        if ((size_t)n >= cache_cap) {
            size_t new_cap = cache_cap;
            while ((size_t)n >= new_cap)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms using the linear recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t v1 = cache[i-1];
            seq_t v2 = cache[i-2];
            seq_t v3 = cache[i-3];
            seq_t v4 = cache[i-4];
            cache[i] = 8*v1 - 12*v2 + 8*v3 - 2*v4;
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
