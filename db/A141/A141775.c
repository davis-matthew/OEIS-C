/*
 * A141775  Binomial transform of (1, 2, 0, 1, 2, 0, 1, 2, 0, ...).
 * https://oeis.org/A141775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) with a(0) = 1, a(1) = 3,
 *            a(2) = 5. Sequence A141775 (Binomial transform of
 *            (1,2,0,1,2,0,…))
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 31, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Basic terms */
    static const seq_t base[3] = { (seq_t)1, (seq_t)3, (seq_t)5 };

    if (n < 3)
        return base[n];

    /* Static cache for previously computed values */
    static seq_t *cache      = NULL;   /* cache[0 … cache_max] */
    static size_t cache_cap  = 0;      /* allocated size */
    static idx_t  cache_max  = 0;      /* largest n currently stored */

    /* Allocate initial cache (3 entries) on first call */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* cannot continue without memory */
        cache[0] = base[0];
        cache[1] = base[1];
        cache[2] = base[2];
        cache_max = 2;
    }

    /* Expand cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries up to n */
    if (n > cache_max) {
        for (size_t i = (size_t)(cache_max + 1); i <= (size_t)n; ++i) {
            seq_t t1 = (seq_t)3 * cache[i - 1];
            seq_t t2 = (seq_t)3 * cache[i - 2];
            seq_t t3 = (seq_t)2 * cache[i - 3];
            cache[i] = t1 - t2 + t3;
        }
        cache_max = n;
    }

    return cache[(size_t)n];
}
