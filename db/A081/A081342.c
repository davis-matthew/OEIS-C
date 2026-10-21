/*
 * A081342  a(n) = (8^n + 2^n)/2.
 * https://oeis.org/A081342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = (8^n + 2^n)/2 = 10·a(n‑1) – 16·a(n‑2), a(0)=1, a(1)=5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache (0‑based) initialized lazily */
    static seq_t *cache = NULL;
    static idx_t cache_last = 1;   /* last index with a valid value */
    static idx_t cache_size = 0;   /* allocated size */

    /* initialise cache */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();          /* should never happen in test harness */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache_last = 1;
    }

    /* grow cache if necessary (double strategy) */
    if ((idx_t)n >= cache_size) {
        idx_t new_size = cache_size ? cache_size << 1 : 2;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute values up to n if not already present */
    if ((idx_t)n > cache_last) {
        for (idx_t k = cache_last + 1; k <= (idx_t)n; ++k) {
            /* a(k) = 10*a(k-1) - 16*a(k-2) */
            seq_t t1 = (seq_t)10 * cache[k - 1];
            seq_t t2 = (seq_t)16 * cache[k - 2];
            cache[k] = t1 - t2;
        }
        cache_last = n;
    }

    return cache[n];
}
