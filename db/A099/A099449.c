/*
 * A099449  An Alexander sequence for the knot 7_6.
 * https://oeis.org/A099449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 5*a(n-3) - a(n-4) for n > 4 Initial
 *            values: a(0)=1, a(1)=5, a(2)=18, a(3)=60, a(4)=197
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_alloc = 0;   /* allocated size */
    static size_t cache_used = 0;    /* number of values stored */

    /* initial terms */
    static const seq_t init_vals[5] = {1, 5, 18, 60, 197};

    /* ensure we have room for at least (n+1) values */
    if ((size_t)n + 1 > cache_alloc) {
        size_t new_alloc = cache_alloc ? cache_alloc : 16;
        while ((size_t)n + 1 > new_alloc)
            new_alloc *= 2;
        cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        cache_alloc = new_alloc;
    }

    /* if cache is empty, fill in the first five values */
    if (cache_used == 0) {
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
        cache_used = 5;
    }

    /* if requested index is already computed, return it */
    if ((size_t)n < cache_used)
        return cache[(size_t)n];

    /* compute values up to n */
    for (size_t i = cache_used; i <= (size_t)n; ++i) {
        seq_t val;
        if (i < 5)
            val = init_vals[i];
        else {
            /* recurrence: 5*a(i-1) - 7*a(i-2) + 5*a(i-3) - a(i-4) */
            val = ((seq_t)5 * cache[i-1])
                - ((seq_t)7 * cache[i-2])
                + ((seq_t)5 * cache[i-3])
                -  cache[i-4];
        }
        cache[i] = val;
    }
    cache_used = (size_t)n + 1;
    return cache[(size_t)n];
}
