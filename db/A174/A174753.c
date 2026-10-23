/*
 * A174753  x-values in the solution to x^2-43*y^2=1.
 * https://oeis.org/A174753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6964*a(n-1) - a(n-2) with a(1)=1, a(2)=3482
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;   /* number of terms stored */
    static size_t  cache_cap = 0;   /* allocated capacity */

    /* required number of entries */
    size_t needed = (size_t)n;

    /* enlarge cache if needed */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;          /* a(1) */
        else if (i == 1)
            val = (seq_t)3482;       /* a(2) */
        else
            val = ((seq_t)6964 * cache[i-1]) - cache[i-2];
        cache[i] = val;
    }
    cache_len = n;

    return cache[n-1];
}
