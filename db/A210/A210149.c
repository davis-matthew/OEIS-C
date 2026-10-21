/*
 * A210149  1/4 the number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          having one, three or four distinct clockwise edge differences.
 * https://oeis.org/A210149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) + 46*a(n-2) + 36*a(n-3) - 10*a(n-4) - 8*a(n-5)
 *            (n ≥ 6), a(1)=57, a(2)=820, a(3)=11783, a(4)=169343,
 *            a(5)=2433709
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <algorithm>
#endif

seq_t a(idx_t n)
{
    /* 1‑based cache of computed values */
    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;   /* highest index that has a value */
    static idx_t  cache_cap  = 0;   /* allocated capacity (max index + 1) */

    if (n == 0)      /* undefined, return 0 for safety */
        return 0;

    /* First call: allocate some capacity and initialise the first 5 terms */
    if (cache == NULL) {
        cache_cap = 64;
        #ifdef __cplusplus
        cache = new seq_t[cache_cap + 1];
        #else
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        #endif
        /* initialise indices 1..5 */
        cache[1] = (seq_t)57;
        cache[2] = (seq_t)820;
        cache[3] = (seq_t)11783;
        cache[4] = (seq_t)169343;
        cache[5] = (seq_t)2433709;
        cache_len = 5;
    }

    /* If the requested index is already cached, return it. */
    if (n <= cache_len)
        return cache[n];

    /* Resize the cache if necessary (never shrink). */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < n)
            new_cap <<= 1;   /* double until it can hold n */

        #ifdef __cplusplus
        seq_t *new_arr = new seq_t[new_cap + 1];
        /* copy existing values */
        std::copy(cache, cache + cache_len + 1, new_arr);
        delete[] cache;
        cache = new_arr;
        #else
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        /* realloc may preserve contents; no need to copy manually */
        #endif
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n (recurrence requires the five
       previous terms; the first five are already set). */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        /* recurrence: a(i) = 11*a1 + 46*a2 + 36*a3 - 10*a4 - 8*a5 */
        cache[i] =
            11 * a1 + 46 * a2 + 36 * a3 - 10 * a4 - 8 * a5;
    }
    cache_len = n;

    return cache[n];
}
