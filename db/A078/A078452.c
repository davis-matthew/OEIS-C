/*
 * A078452  a(1)=1, a(n) = ceiling(n/a(a(a(n-1)))).
 * https://oeis.org/A078452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( n / a( a( a(n-1) ) ) )
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Integers are 1‑based; a(1)=1.  For n>=2 the recurrence holds.) */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed values, 1‑based internally */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of computed terms */

    /* Expand cache if necessary and fill new terms */
    if (n > cache_len) {
        /* Allocate space for up to n terms */
        seq_t *new_cache = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) { /* out‑of‑memory; return 0 as sentinel (not expected) */
            free(cache);
            cache = NULL;
            cache_len = 0;
            return (seq_t)0;
        }
        cache = new_cache;

        /* Compute terms from cache_len+1 up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else {
                /* indices are 1‑based, but array is 0‑based */
                seq_t j = cache[(size_t)i - 2];          /* a(i-1)   */
                seq_t k = cache[(size_t)j - 1];          /* a(j)     */
                seq_t l = cache[(size_t)k - 1];          /* a(k)     */
                seq_t t = l;                             /* a(a(a(i-1))) */

                /* a(i) = ceil(i / t) = (i + t - 1) / t  (all seq_t) */
                seq_t i_seq = (seq_t)i;
                val = (i_seq + t - (seq_t)1) / t;
            }
            cache[(size_t)i - 1] = val;
        }
        cache_len = n; /* update known length */
    }

    return cache[(size_t)n - 1];
}
