/*
 * A207304  Number of n X 7 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) with a(1)=28, a(2)=784, a(3)=4344, a(4)=14446,
 *            a(5)=36868, a(6)=79802
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 64, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside OEIS offset, harmless */

    /* static cache for memoisation: cache[i] = a(i+1) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* number of stored terms */

    /* initialise cache with the first 6 terms */
    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)28;     /* a(1) */
        cache[1] = (seq_t)784;    /* a(2) */
        cache[2] = (seq_t)4344;   /* a(3) */
        cache[3] = (seq_t)14446;  /* a(4) */
        cache[4] = (seq_t)36868;  /* a(5) */
        cache[5] = (seq_t)79802;  /* a(6) */
    }

    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* grow cache to hold up to n terms */
    if (cache_size < (size_t)n) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            n * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* allocation failure */
        cache = new_cache;
        /* compute missing terms from the old size up to n-1 */
        size_t old_size = cache_size;
        cache_size = (size_t)n;
        for (size_t idx = old_size; idx < cache_size; ++idx) {
            /* terms needed: a(idx+1) = 6*a(idx) - 15*a(idx-1) + 20*a(idx-2)
                                 - 15*a(idx-3) + 6*a(idx-4) - a(idx-5) */
            seq_t term0 = cache[idx - 1];   /* a(idx)   */
            seq_t term1 = cache[idx - 2];   /* a(idx-1) */
            seq_t term2 = cache[idx - 3];   /* a(idx-2) */
            seq_t term3 = cache[idx - 4];   /* a(idx-3) */
            seq_t term4 = cache[idx - 5];   /* a(idx-4) */
            seq_t term5 = cache[idx - 6];   /* a(idx-5) */

            seq_t add_term = term0 * (seq_t)6 + term2 * (seq_t)20
                           + term4 * (seq_t)6;
            seq_t sub_term = term1 * (seq_t)15 + term3 * (seq_t)15
                           + term5;

            cache[idx] = add_term - sub_term;
        }
    }

    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];
    return (seq_t)0;   /* should never reach here */
}
