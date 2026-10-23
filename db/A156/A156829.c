/*
 * A156829  a(1) = 2. a(n) = the smallest prime >= a(n-1) + 6.
 * https://oeis.org/A156829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A156829
    a(1) = 2
    a(n) = the smallest prime ≥ a(n-1) + 6   (n > 1)
    The implementation caches all computed terms; after the
    necessary values have been generated every call is O(1).
    It works with any seq_t / idx_t implementation supplied by the
    build and is fully compliant with the C11/C++17 requirements.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *cache   = NULL;   /* 1‑indexed array: cache[0] = a(1) */
    static idx_t cache_len = 0;     /* number of elements stored  */
    static idx_t cache_cap = 0;     /* allocated capacity          */

    if (n == 0) {                   /* undefined for 0, but return 0 */
        return 0;
    }

    /* first call: initialise the cache with a(1) */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failure */
        cache[0] = (seq_t)2;        /* a(1) = 2 */
        cache_len = 1;
    }

    /* grow the cache if the requested index is outside the current
       stored range.  Double the capacity each time until it is large
       enough.  realloc is safe because we never shrink the allocation. */
    if (n > cache_len) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;

        /* compute all missing terms up to n */
        for (idx_t i = cache_len; i < n; ++i) {
            /* start searching at a(i-1) + 6 */
            seq_t cand = cache[i-1] + (seq_t)6;
            while (!is_prime_big(cand)) {
                cand += (seq_t)1;
            }
            cache[i] = cand;
        }
        cache_len = n;
    }

    /* sequence is 1‑indexed: a(n) is stored at cache[n-1] */
    return cache[n-1];
}
