/*
 * A224670  Number of (n+1) X 3 0..2 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 0..2 matrices with idempotent 2×2
 *            subblocks. Recurrence: a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) +
 *            2*a(n-4) - a(n-5), with initial terms a(1)=25, a(2)=50, a(3)=76,
 *            a(4)=123, a(5)=191
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization.  cache[i] holds a(i). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (max index) */
    static size_t cache_cap = 0;  /* allocated capacity (size of cache array) */

    /* Initial capacity and first five terms.  Index 0 unused. */
    if (cache == NULL) {
        cache_cap = 6;                    /* 0..5 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 25;
        cache[2] = 50;
        cache[3] = 76;
        cache[4] = 123;
        cache[5] = 191;
        cache_len = 5;
    }

    if (n <= cache_len)
        return cache[n];

    /* Grow cache as needed. */
    if (n >= cache_cap) {
        /* Allocate enough room for all terms up to n. */
        size_t new_cap = n + 1;
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* allocation failure – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = 4 * cache[i - 1]
                    - 5 * cache[i - 2]
                    + cache[i - 3]
                    + 2 * cache[i - 4]
                    - cache[i - 5];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
