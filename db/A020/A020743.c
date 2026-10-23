/*
 * A020743  Pisot sequence L(7,10).
 * https://oeis.org/A020743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3); a(0)=7, a(1)=10, a(2)=15
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache: values a(0)..a(last) */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static idx_t   cache_last = (idx_t)-1;   /* no values yet */

    /* Ensure cache holds indices 0..max(n,2) */
    size_t needed = (size_t)(n + 1);
    if (needed < 3) needed = 3;          /* at least 0,1,2 must exist */

    if (cache == NULL) {                 /* First use: allocate and init */
        cache_cap = needed;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);             /* allocation failure */
        cache[0] = (seq_t)7;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)15;
        cache_last = 2;
    } else if (needed > cache_cap) {     /* Grow cache if needed */
        size_t new_cap = cache_cap;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    if ((idx_t)cache_last < (idx_t)3) {
        /* cache_last may be -1 if this is the first call with n<3 */
        if ((idx_t)cache_last < (idx_t)2) cache_last = 2;
    }

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = ((seq_t)2 * cache[i - 1]) - cache[i - 3];
    }
    cache_last = n;
    return cache[n];
}
