/*
 * A103379  a(n) = a(n-11) + a(n-12).
 * https://oeis.org/A103379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-11) + a(n-12),  a(1)=a(2)=…=a(12)=1  (offset 1)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization (1‑based indices) */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;      /* number of elements allocated (index 0 unused) */
    static idx_t  last_computed = 0;   /* largest already computed index          */

    if (n == 0) return (seq_t)0;   /* optional: guard against zero index */

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);            /* indices 0 … n */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                           new_size * sizeof(seq_t));
        if (!new_cache) abort();                     /* allocation failure */
        cache = new_cache;
        cache_size = new_size;

        /* initialise the first 12 terms (if within bounds) */
        for (idx_t i = 1; i <= 12 && i < (idx_t)cache_size; ++i) {
            cache[i] = (seq_t)1;
        }
    }

    /* compute missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i <= 12)
                cache[i] = (seq_t)1;
            else
                cache[i] = cache[i - 11] + cache[i - 12];
        }
        last_computed = n;
    }

    return cache[n];
}
