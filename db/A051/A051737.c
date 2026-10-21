/*
 * A051737  Number of 4 X n (0,1)-matrices with no consecutive 1's in any row
 *          or column.
 * https://oeis.org/A051737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + 9*a(n-2) - 5*a(n-3) - 4*a(n-4) + a(n-5);
 *            Initial values: a(0)=1, a(1)=8, a(2)=41, a(3)=227, a(4)=1234
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[5] = {
    (seq_t)1, (seq_t)8, (seq_t)41, (seq_t)227, (seq_t)1234
};

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* allocated capacity   */
    static size_t filled = 0;     /* number of terms stored   */

    /* Ensure we can store indices up to n */
    if (!cache) {
        size_t needed = n + 1;
        if (needed < 5) needed = 5;
        cache_len = needed;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        /* initialise first five terms */
        for (size_t i = 0; i < 5; ++i) cache[i] = init_vals[i];
        for (size_t i = 5; i < cache_len; ++i) cache[i] = (seq_t)0;
        filled = 5;
    } else if ((size_t)n + 1 > cache_len) {
        /* grow cache to at least n+1 */
        size_t new_len = cache_len;
        while (new_len <= (size_t)n) new_len <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialise new slots to zero */
        for (size_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;
        cache_len = new_len;
    }

    /* Compute missing terms up to n if needed */
    if ((size_t)n >= filled) {
        for (size_t i = filled; i <= (size_t)n; ++i) {
            if (i < 5) continue;                /* already initialised */
            cache[i] = 4 * cache[i - 1]
                     + 9 * cache[i - 2]
                     - 5 * cache[i - 3]
                     - 4 * cache[i - 4]
                     +   cache[i - 5];
        }
        if ((size_t)n + 1 > filled) filled = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
