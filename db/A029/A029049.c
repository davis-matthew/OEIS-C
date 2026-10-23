/*
 * A029049  Expansion of 1/((1-x)(1-x^3)(1-x^7)(1-x^8)).
 * https://oeis.org/A029049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1,3,7,8. Recurrence:
 *            a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-7) - a(n-9) - a(n-10) +
 *            a(n-12) - a(n-15) + a(n-16) + a(n-18) - a(n-19) with a(k<0)=0,
 *            a(0)=1
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* ensure capacity for index n (indices 0..n) */
    if (n >= (idx_t)(cache_cap)) {
        size_t new_cap = cache_cap ? (size_t)cache_cap * 2 : 16;
        while ((idx_t)new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        /* zero any newly allocated elements (not strictly necessary) */
        if (new_cap > (size_t)cache_cap)
            for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* initialise first term */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 1) val += cache[i-1];
        if (i >= 3) val += cache[i-3];
        if (i >= 4) val -= cache[i-4];
        if (i >= 7) val += cache[i-7];
        if (i >= 9) val -= cache[i-9];
        if (i >=10) val -= cache[i-10];
        if (i >=12) val += cache[i-12];
        if (i >=15) val -= cache[i-15];
        if (i >=16) val += cache[i-16];
        if (i >=18) val += cache[i-18];
        if (i >=19) val -= cache[i-19];
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
