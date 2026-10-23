/*
 * A025851  Expansion of 1/((1-x^3)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A025851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A025851: number of partitions of n into parts 3, 8, and 10.
    Recurrence (for n ≥ 0):
            a(0) = 1
            a(n) = a(n-3) + a(n-8) + a(n-10)
                  – a(n-11) – a(n-13) – a(n-18)
                  + a(n-21)
    (Terms with negative indices are taken to be 0.) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* cache holds indices 0 .. cache_size-1 */

    /* initialise cache with a(0) = 1 */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* grow cache and compute missing values up to n */
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        /* compute all a(i) for i from cache_size to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 0;
            if (i >= (idx_t)3)   val += cache[i - 3];
            if (i >= (idx_t)8)   val += cache[i - 8];
            if (i >= (idx_t)10)  val += cache[i -10];
            if (i >= (idx_t)11)  val -= cache[i -11];
            if (i >= (idx_t)13)  val -= cache[i -13];
            if (i >= (idx_t)18)  val -= cache[i -18];
            if (i >= (idx_t)21)  val += cache[i -21];
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
