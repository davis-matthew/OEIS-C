/*
 * A081555  a(n) = 6*a(n-1) - a(n-2) - 4, a(0)=3, a(1)=7.
 * https://oeis.org/A081555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 4, a(0) = 3, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed iteratively with memoisation.  The cached
 * values are stored in a static array that grows as needed.  The
 * recurrence is linear and grows rapidly, so the computation is
 * O(n) and well below the allowed time for n up to about 10000.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i).  cache_cap is the number of
     * elements currently stored.  We initialise with the two
     * initial values and extend the cache when a larger n is requested.
     */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Initialise the cache with a(0)=3, a(1)=7 */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* out‑of‑memory: return 0 as a safe fallback */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)7;
    }

    /* Grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out‑of‑memory: return 0 as a safe fallback */
        cache = tmp;

        /* Compute missing terms up to n */
        for (size_t i = cache_cap; i <= (size_t)n; ++i) {
            /* a(i) = 6*a(i-1) - a(i-2) - 4  with unsigned arithmetic */
            seq_t val = 6 * cache[i-1] - cache[i-2] - (seq_t)4;
            cache[i] = val;
        }
        cache_cap = new_cap;
    }
    return cache[(size_t)n];
}
