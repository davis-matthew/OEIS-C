/*
 * A199773  y-values in the solution to 17*x^2 - 16 = y^2.
 * https://oeis.org/A199773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66*a(n-3) - a(n-6), with a(1)=1, a(2)=16, a(3)=103,
 *            a(4)=169, a(5)=1072, a(6)=6799
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds all terms that have been computed so far */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* maximum index that can be addressed */

    /* helper to grow the cache to index `req` (inclusive) */
    if (cache == NULL) {
        /* start with capacity for at least 6 terms */
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap + 1, sizeof(seq_t));
        if (!cache) return 0; /* out of memory, but caller won't expect this */
        /* initialise the first six terms */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)103;
        cache[4] = (seq_t)169;
        cache[5] = (seq_t)1072;
        cache[6] = (seq_t)6799;
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        /* zero the new part */
        for (size_t i = cache_cap + 1; i <= new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n in increasing order */
    for (size_t i = 7; (size_t)i <= (size_t)n; ++i) {
        if (!cache[i]) { /* uncomputed yet */
            seq_t term = (seq_t)66 * cache[i - 3] - cache[i - 6];
            cache[i] = term;
        }
    }

    return cache[n];
}
