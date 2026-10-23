/*
 * A192312  1-sequence of reduction of (3n-2) by x^2 -> x+1.
 * https://oeis.org/A192312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A192312
 *  Recurrence:  a(1)=0, a(2)=4, a(3)=11, a(4)=31, a(5)=70,
 *               a(n)=3*a(n-1)-a(n-2)-3*a(n-3)+a(n-4)+a(n-5) for n≥6.
 *  The function memoises terms so that calls in any order up to n≈10000
 *  run in O(n) time and use O(n) memory.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Cached terms (index 0 is unused). */
    static seq_t *cache   = NULL;      /* array of computed a(i)  */
    static idx_t  cache_len = 0;       /* highest index computed   */
    static size_t cache_cap = 0;       /* current capacity         */

    if (n < 1) return 0;   /* behaviour below offset is irrelevant */

    /* Initialise on first call.  Allocate a small initial capacity. */
    if (cache == NULL) {
        cache_cap = 16;                /* start with room for indices 0..15 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) exit(1);           /* fatal if allocation fails   */
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = 0;

        /* Known initial values */
        cache[1] = (seq_t)0;    /* a(1) */
        cache[2] = (seq_t)4;    /* a(2) */
        cache[3] = (seq_t)11;   /* a(3) */
        cache[4] = (seq_t)31;   /* a(4) */
        cache[5] = (seq_t)70;   /* a(5) */
        cache_len = 5;
    }

    /* Grow the cache if needed and compute any missing terms. */
    if (n > cache_len) {
        /* Ensure the array can hold indices up to n. */
        if ((size_t)(n + 1) > cache_cap) {
            size_t new_cap = cache_cap;
            while ((size_t)(n + 1) > new_cap) new_cap <<= 1;   /* double */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);                                 /* failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute terms up to n using the recurrence. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val = 3 * cache[i-1];
            val -= cache[i-2];
            val -= 3 * cache[i-3];
            val += cache[i-4];
            val += cache[i-5];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
