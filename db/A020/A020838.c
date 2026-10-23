/*
 * A020838  Expansion of 1/((1-7x)(1-8x)(1-10x)).
 * https://oeis.org/A020838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=25, a(2)=419; *
 *            a(n)=25*a(n-1)-206*a(n-2)+560*a(n-3) for n>=3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed values   */
    static size_t cache_len = 0;  /* number of values stored    */
    static size_t cache_cap = 0;  /* allocated capacity         */

    /* initialise cache – first three terms provided by OEIS */
    if (cache_len == 0) {
        size_t init_cap = 8;                     /* small initial capacity   */
        cache = (seq_t*)calloc(init_cap, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)419;
        cache_len = 3;
    }

    /* if the requested index is already cached, just return it */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* make sure the array can hold up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill in all missing terms using the 3‑term recurrence */
    for (size_t i = cache_len; i < needed; ++i) {
        cache[i] =
            (seq_t)25  * cache[i-1]
          - (seq_t)206 * cache[i-2]
          + (seq_t)560 * cache[i-3];
    }
    cache_len = needed;

    return cache[(size_t)n];
}
