/*
 * A240835  a(n)=1 for n <= s+k; thereafter a(n) = Sum_{i=0..k-1}
 *          a(n-i-s-a(n-i-1)) where s=0, k=4.
 * https://oeis.org/A240835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = 1 for n <= 4 * a(n) = Σ_{i=0..3} a(n - i - a(n-i-1)) (n >
 *            4)
 * Range:     u8 n <= 337, u16 n <= 9154, u32 n <= 9154, u64 n <= 9154,
 *            u128 n <= 9154, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoises previously computed values to allow calls in any order. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i >= 0 (cache[0] unused). */
    static seq_t *cache = NULL;
    static size_t cap  = 0;          /* allocated size */
    static idx_t last  = 0;          /* last computed index */

    /* Grow the cache if necessary – at least n+1 elements (including unused 0). */
    if (cap <= (size_t)n) {
        size_t newcap = (cap ? cap << 1 : 16);
        if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)calloc(newcap, sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failure (unlikely) */
        if (cache) {
            /* copy already cached values */
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t m = last + 1; m <= n; ++m) {
        if (m <= 4) {
            cache[m] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            for (int i = 0; i < 4; ++i) {
                /* a(m-i-1) */
                seq_t prev = cache[m - i - 1];
                /* j = m - i - prev (index needs a machine integer) */
                idx_t j;
#ifdef __cplusplus
                j = (idx_t)m - i - static_cast<idx_t>(prev);
#else
                j = (idx_t)m - i - (idx_t)prev;
#endif
                if (j < 1) j = 1;                /* safety – should not happen */
                sum += cache[j];
            }
            cache[m] = sum;
        }
    }
    last = n;

    return cache[n];
}
