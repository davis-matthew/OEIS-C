/*
 * A129871  A variant of Sylvester's sequence: a(0)=1 and for n>0, a(n) =
 *          (a(0)*a(1)*...*a(n-1)) + 1.
 * https://oeis.org/A129871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(n+1)=a(n)^2 - a(n) + 1 (for n≥1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cachelen = 0;   /* number of terms stored (indices 0..cachelen-1) */

    if (cachelen <= n) {
        idx_t newlen = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)newlen * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed */
        cache = tmp;

        /* initialise base terms if the cache was empty */
        if (cachelen == 0) {
            cache[0] = (seq_t)1;
            cachelen = 1;
        }

        /* ensure a(1)=2 is available when needed */
        if (newlen > 1 && cachelen < 2) {
            cache[1] = (seq_t)2;
            cachelen = 2;
        }

        /* compute any missing terms up to the requested index */
        for (idx_t i = cachelen; i < newlen; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = prev * prev - prev + (seq_t)1;
        }
        cachelen = newlen;
    }
    return cache[n];
}
