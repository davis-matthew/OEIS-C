/*
 * A027637  a(n) = Product_{i=1..n} (4^i - 1).
 * https://oeis.org/A027637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1..n} (4^i − 1) (a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization state */
    static seq_t *cache       = NULL;   /* array of computed terms */
    static size_t cache_cap   = 0;      /* allocated capacity */
    static size_t cached_terms = 0;     /* number of terms currently stored (cache[0..cached_terms-1]) */
    static seq_t last_power4 = (seq_t)1;/* holds 4^(cached_terms-1) */

    /* First-time initialization: cache[0] = 1 */
    if (cached_terms == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();           /* allocation failure → abort */
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        cached_terms = 1;              /* one term stored */
    }

    /* Extend cache up to n, if necessary */
    if ((size_t)n + 1 > cached_terms) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {      /* grow capacity */
            size_t new_cap = cache_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms */
        for (size_t i = cached_terms; i <= (size_t)n; ++i) {
            last_power4 *= (seq_t)4;         /* 4^i */
            seq_t val = cache[i-1] * (last_power4 - (seq_t)1);
            cache[i] = val;
        }
        cached_terms = needed;
    }

    return cache[(size_t)n];
}
