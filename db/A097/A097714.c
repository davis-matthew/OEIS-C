/*
 * A097714  Repeatedly convert from sexagesimal to centesimal, starting with
 *          60.
 * https://oeis.org/A097714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 100 * floor(a(n‑1)/60) + a(n‑1)%60, a(1) = 60
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence on demand, storing previously
   computed terms in a static cache. All arithmetic is performed in seq_t
   using only the allowed operators. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* cache[0] holds a(1) */
    static idx_t  cache_len = 0;      /* number of terms currently cached */

    /* Ensure cache can hold at least n terms (1‑based index). */
    if (n > 0 && (size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return 0;           /* allocation failed – return 0 as placeholder */

        cache = tmp;

        /* If this is the first allocation, initialise the first term. */
        if (cache_len == 0) {
            cache[0] = (seq_t)60;      /* a(1) */
            cache_len = 1;
        }

        /* Compute any missing terms up to n. */
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t h    = prev / (seq_t)60;   /* hours part   */
            seq_t m    = prev % (seq_t)60;   /* minutes part */
            cache[i]   = (seq_t)100 * h + m;/* a(i+1) */
        }
        cache_len = n;
    }

    /* If n is zero the sequence is undefined; return 0. */
    if (n == 0) return 0;

    /* Return the requested term (n is 1‑based). */
    return cache[n - 1];
}
