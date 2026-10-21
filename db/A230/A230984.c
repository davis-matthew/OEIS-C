/*
 * A230984  Number of white square subarrays of (n+1) X (3+1) binary arrays
 *          with no element equal to a strict majority of its diagonal and
 *          antidiagonal neighbors, with upper left element zero.
 * https://oeis.org/A230984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)+3*a(n-2)-3*a(n-3)-6*a(n-4)+2*a(n-5)+4*a(n-6)-a(n-7)-a(n-8)
 *            Initial values: a(1)=2, a(2)=4, a(3)=11, a(4)=24, a(5)=59,
 *            a(6)=139, a(7)=332, a(8)=796
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* cache[0] holds a(1) */
    static size_t cache_len = 0;     /* number of terms stored (>=1) */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* initialise cache with the first 8 terms if needed */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failed */
        static const seq_t init[8] = {
            (seq_t)2, (seq_t)4, (seq_t)11, (seq_t)24,
            (seq_t)59, (seq_t)139, (seq_t)332, (seq_t)796
        };
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_len = 8;
    }

    if (n == 0) return (seq_t)0;

    if (n <= cache_len)
        return cache[n - 1];

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;           /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute terms from cache_len+1 up to n using the 8‑term linear recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos = 2 * cache[i - 2] + 3 * cache[i - 3]
                  + 2 * cache[i - 6] + 4 * cache[i - 7];
        seq_t neg = 3 * cache[i - 4] + 6 * cache[i - 5]
                  + cache[i - 8] + cache[i - 9];
        cache[i - 1] = pos - neg; /* pos ≥ neg for all valid n */
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
