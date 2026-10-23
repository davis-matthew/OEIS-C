/*
 * A122997  Pentanacci numbers for following initial values: a(0) = 1, a(1) =
 *          -1, a(2) = 1, a(3) = -1, a(4) = 1.
 * https://oeis.org/A122997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5) * Initial
 *            values: a(0)=1, a(1)=-1, a(2)=1, a(3)=-1, a(4)=1
 * Range:     i8 n <= 12, i16 n <= 20, i32 n <= 37, i64 n <= 70, i128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of valid entries (next index to compute) */

    /* lazy initialization of the first five terms */
    if (!cache) {
        cache_cap = 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* fallback on allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)-1;
        cache[4] = (seq_t)1;
        cache_len = 5;
    }

    /* grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* fallback on reallocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    while (cache_len <= (size_t)n) {
        if (cache_len >= 5) {
            seq_t sum = cache[cache_len - 1];
            sum += cache[cache_len - 2];
            sum += cache[cache_len - 3];
            sum += cache[cache_len - 4];
            sum += cache[cache_len - 5];
            cache[cache_len] = sum;
        }
        /* for indices <5 the value is already set; just advance the index */
        ++cache_len;
    }

    return cache[(size_t)n];
}
