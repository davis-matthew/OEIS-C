/*
 * A181961  Number of independent sets of nodes in graph C_6 X P_n (n>=0).
 * https://oeis.org/A181961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A181961
   Recurrence: a(n) = 12*a(n-1)-3*a(n-2)-25*a(n-3)-2*a(n-4)+a(n-5)
   with initial values a(0)=1, a(1)=18, a(2)=199, a(3)=2309, a(4)=26660
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = 0;
    static size_t cache_size = 0;           /* allocated capacity (>= max index + 1) */
    static idx_t  max_cached = (idx_t)-1;   /* highest index that has been computed */

    /* initialise cache with initial terms on first call */
    if (!cache) {
        enum { INIT_CAP = 8 };
        cache = (seq_t*)calloc(INIT_CAP, sizeof(seq_t));
        if (!cache)
            return (seq_t)0;               /* allocation failure – return 0 */

        cache_size = INIT_CAP;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)18;
        cache[2] = (seq_t)199;
        cache[3] = (seq_t)2309;
        cache[4] = (seq_t)26660;
        max_cached = 4;
    }

    /* if already computed, return immediately */
    if (n <= max_cached)
        return cache[n];

    /* grow the cache to hold up to index n */
    size_t required = (size_t)(n + 1);
    if (required > cache_size) {
        size_t new_cap = cache_size ? cache_size : 8;
        while (new_cap < required)
            new_cap <<= 1;                 /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                          /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_size = new_cap;
    }

    /* compute any missing terms using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] =
            12 * cache[i-1]
          - 3  * cache[i-2]
          - 25 * cache[i-3]
          - 2  * cache[i-4]
          + cache[i-5];
    }
    max_cached = n;
    return cache[n];
}
