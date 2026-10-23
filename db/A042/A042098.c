/*
 * A042098  Numerators of continued fraction convergents to sqrt(573).
 * https://oeis.org/A042098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 766*a(n-4) - a(n-8), with initial values a(0)=23,
 *            a(1)=24, a(2)=359, a(3)=383, a(4)=17977, a(5)=18360,
 *            a(6)=275017, a(7)=293377
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"   /* provides void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr); */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* capacity = number of indices that can be stored (n+1) */
    static idx_t last = (idx_t)-1;        /* largest index that has been computed;
                                            (idx_t)-1 indicates that nothing has been computed yet */

    /* initial eight values */
    static const seq_t init[8] = {
        (seq_t)23,
        (seq_t)24,
        (seq_t)359,
        (seq_t)383,
        (seq_t)17977,
        (seq_t)18360,
        (seq_t)275017,
        (seq_t)293377
    };

    /* make sure the cache can hold indices up to n */
    if (!cache || n >= cache_cap) {
        ensure_cache(n, &cache, &cache_cap);   /* guarantees cache_cap > n */
    }

    /* if we haven't computed up to n yet, do it */
    if (last == (idx_t)-1 || n > last) {
        /* start index for computation */
        idx_t start = (last == (idx_t)-1) ? (idx_t)0 : (last + (idx_t)1);

        for (idx_t i = start; i <= n; ++i) {
            if (i < 8) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(i) = 766*a(i-4) - a(i-8) */
                cache[i] = ((seq_t)766 * cache[i - (idx_t)4]) - cache[i - (idx_t)8];
            }
        }
        last = n;
    }

    return cache[n];
}
