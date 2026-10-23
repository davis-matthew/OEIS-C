/*
 * A017999  Expansion of 1/((1-3x)*(1-7x)*(1-10x)).
 * https://oeis.org/A017999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=20, a(2)=279,
   a(n)=20*a(n-1)-121*a(n-2)+210*a(n-3)  (n>2) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated size of cache array   */
    static size_t cache_len = 0;          /* number of terms actually stored */

    /* If cache is not yet allocated, start with a small capacity */
    if (!cache_cap) {
        cache_cap = 8;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) abort();              /* memory allocation failed */
    }

    /* Grow cache in powers of two until it can hold index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the very first terms if they are not already stored */
    if (cache_len <= 0) { cache[0] = (seq_t)1; cache_len = 1; }
    if (cache_len <= 1) { cache[1] = (seq_t)20; cache_len = 2; }
    if (cache_len <= 2) { cache[2] = (seq_t)279; cache_len = 3; }

    /* Compute missing terms up to and including n */
    if (n >= (idx_t)cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0)   { cache[i] = (seq_t)1; }
            else if (i == 1) { cache[i] = (seq_t)20; }
            else if (i == 2) { cache[i] = (seq_t)279; }
            else {
                /* Use recurrence: a(i)=20*a(i-1)-121*a(i-2)+210*a(i-3) */
                seq_t t1 = cache[i-1] * (seq_t)20;
                seq_t t2 = cache[i-2] * (seq_t)121;
                seq_t t3 = cache[i-3] * (seq_t)210;
                cache[i] = t1 - t2 + t3;
            }
        }
        cache_len = (size_t)n + 1;    /* all terms up to n are now stored */
    }

    return cache[(size_t)n];
}
