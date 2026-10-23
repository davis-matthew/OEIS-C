/*
 * A126391  a(1)=1; for n>1: a(n) = sum of all subsets of (a(1),..,a(n-1)).
 * https://oeis.org/A126391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1; for n>2: a(n) = (2^(n-2)+2) * a(n-1)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is also A006088(n-1) */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cached_len = 0;       /* number of entries stored (>=1) */
    static seq_t power_of_two = 0;      /* 2^(k-2) for most recently computed k */
    static idx_t  last_k       = 0;     /* largest n that has been computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4;                     /* initial allocation size */
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* out of memory – fall back to 0 */
        cache[0] = (seq_t)1;               /* a(1) */
        cache[1] = (seq_t)1;               /* a(2) */
        cached_len = 2;
        last_k = 2;
        power_of_two = (seq_t)1;           /* 2^(2-2) = 1 */
    }

    /* Return already known value if requested */
    if (n <= cached_len && n >= 1)
        return cache[n-1];

    /* Expand cache to at least size n */
    if (n > cached_len) {
        size_t new_cap = cache_cap;
        if (new_cap < 4) new_cap = 4;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* out of memory – fall back to 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = last_k + 1; k <= n; ++k) {
        /* update power_of_two to 2^(k-2)   */
        power_of_two = power_of_two * (seq_t)2;   /* power_of_two *= 2 */

        /* factor = power_of_two + 2 = 2^(k-2)+2   */
        seq_t factor = power_of_two + (seq_t)2;

        /* a(k) = a(k-1) * factor   */
        cache[k-1] = cache[k-2] * factor;
    }

    /* Update state markers */
    last_k = n;
    cached_len = n > cached_len ? n : cached_len;

    return cache[n-1];
}
