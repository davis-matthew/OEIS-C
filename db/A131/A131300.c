/*
 * A131300  a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) with n>3, a(0)=1,
 *          a(1)=2, a(2)=3, a(3)=7.
 * https://oeis.org/A131300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) for n>3; initial
 *            values a(0)=1, a(1)=2, a(2)=3, a(3)=7
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small on‑demand cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure the cache is at least large enough for a(0)–a(3) */
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation error */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
    }

    /* Grow the cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation error */
        /* Compute new terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            tmp[i] = (seq_t)(3 * tmp[i - 1]
                            - 2 * tmp[i - 2]
                            - tmp[i - 3]
                            + tmp[i - 4]);
        }
        cache = tmp;
        cache_size = (idx_t)(n + 1);
    }

    return cache[n];
}
