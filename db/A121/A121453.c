/*
 * A121453  Numbers m such that (m mod k) > (m+2 mod k) with least value of k
 *          = 5.
 * https://oeis.org/A121453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4)  (n ≥ 5)
 * initial values: a(1)=9, a(2)=24, a(3)=33, a(4)=48
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset – we never use index 0 */
    if (n == 0)
        return 0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (n ≥ 1) */
    static idx_t cache_cap = 0;   /* allocated capacity (≥ cache_len) */

    /* Initialise cache with first four terms */
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;               /* memory failure – unlikely in tests */
        cache[0] = 9;   /* a(1) */
        cache[1] = 24;  /* a(2) */
        cache[2] = 33;  /* a(3) */
        cache[3] = 48;  /* a(4) */
        cache_len = 4;
    }

    /* If the query is within the cached range, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure capacity for requested index */
    if (n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)n * sizeof(seq_t));
        if (!tmp)
            return 0;               /* memory failure – unlikely */
        cache = tmp;
        cache_cap = n;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t idx = cache_len + 1; idx <= n; ++idx) {
        /* array index = idx - 1 */
        size_t i = (size_t)(idx - 1);
        cache[i] = (seq_t)2 * cache[i - 1]
                 - (seq_t)2 * cache[i - 2]
                 + (seq_t)2 * cache[i - 3]
                 - cache[i - 4];
    }
    cache_len = n;
    return cache[n - 1];
}
