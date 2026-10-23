/*
 * A181247  Number of n X 4 binary matrices with no 2 X 2 block having four
 *          1's.
 * https://oeis.org/A181247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 10*a(n-1) + 54*a(n-2) + 16*a(n-3) -
 *            64*a(n-4) a(1) = 16, a(2) = 216, a(3) = 3032, a(4) = 42176 (OEIS
 *            A181247)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size  = 0;   /* number of terms currently stored */
    static size_t cache_cap   = 0;   /* allocated capacity */

    if (n == 0)   /* offset is 1; undefined for n=0, return 0 */
        return (seq_t)0;

    /* grow cache capacity if needed */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {            /* allocation failed */
            /* In production you might handle this differently */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first terms lazily */
    if (cache_size == 0) {
        cache[0] = (seq_t)16;
        cache[1] = (seq_t)216;
        cache[2] = (seq_t)3032;
        cache[3] = (seq_t)42176;
        cache_size = 4;
    }

    /* compute up to requested n */
    while (cache_size < (size_t)n) {
        /* compute a(current) using the recurrence */
        seq_t val = 10 * cache[cache_size - 1]   /* a(k) */
                  + 54 * cache[cache_size - 2]   /* a(k-1) */
                  + 16 * cache[cache_size - 3];  /* a(k-2) */
        /* subtract the last term */
        val -= 64 * cache[cache_size - 4];      /* a(k-3) */
        cache[cache_size++] = val;
    }

    return cache[n - 1];
}
