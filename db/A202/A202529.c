/*
 * A202529  Number of (n+2) X 7 binary arrays avoiding patterns 000 and 101 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-3)-2*a(n-4)-2*a(n-5)+a(n-7) for n>8
 *            Initial values (offset 1): a(1)=1940, a(2)=4901, a(3)=10240,
 *            a(4)=21297, a(5)=46621, a(6)=100550, a(7)=214388, a(8)=460948
 * Range:     u8 none, u16 n <= 5, u32 n <= 19, u64 n <= 48, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n = 0;
    static size_t cache_cap = 0;

    /* initialize cache with capacity for at least the first 16 entries */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    }

    /* store the first 8 known terms */
    if (max_n < 8) {
        if (cache_cap <= 8) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < 9) new_cap = 9;
            seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (p == NULL) return (seq_t)0;  /* allocation failed, return 0 */
            cache = p;
            cache_cap = new_cap;
            /* zero new entries */
            for (size_t i = max_n + 1; i < cache_cap; ++i) cache[i] = (seq_t)0;
        }
        cache[1] = 1940;
        cache[2] = 4901;
        cache[3] = 10240;
        cache[4] = 21297;
        cache[5] = 46621;
        cache[6] = 100550;
        cache[7] = 214388;
        cache[8] = 460948;
        max_n = 8;
    }

    /* extend cache up to requested n */
    if ((idx_t)n > max_n) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= (idx_t)n) new_cap *= 2;
        if (new_cap > cache_cap) {
            seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (p == NULL) return (seq_t)0; /* allocation failed */
            cache = p;
            for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
            cache_cap = new_cap;
        }
        for (idx_t i = max_n + 1; i <= (idx_t)n; ++i) {
            seq_t val = cache[i-1] * (seq_t)2
                     + cache[i-3] * (seq_t)2
                     - cache[i-4] * (seq_t)2
                     - cache[i-5] * (seq_t)2
                     + cache[i-7];
            cache[i] = val;
        }
        max_n = n;
    }

    return cache[(size_t)n];
}
