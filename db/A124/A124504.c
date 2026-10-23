/*
 * A124504  Number of partitions of an n-set without blocks of size 3.
 * https://oeis.org/A124504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of an n-set without blocks of size
 *            3. Recurrence: a(0) = 1 for n >= 1, a[n] = Σ_{i = 0}^{n-1, i !=
 *            2} C(n-1, i) * a[n-1-i] where C(n-1, i) is the binomial
 *            coefficient
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;       /* array of seq_t values a[0..] */
    static size_t cache_cap = 0;      /* allocated capacity of cache */
    static size_t cache_len = 0;      /* number of values currently stored (cache_len == cached_n+1) */

    /* Ensure cache contains at least a[0] */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) { /* allocation failure */
            exit(1);
        }
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Extend cache up to index n if necessary */
    if ((size_t)n + 1 > cache_len) {
        /* Determine new capacity: double until enough or set to n+1 */
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1) {
            new_cap = new_cap * 2;
            if (new_cap == 0) new_cap = 1; /* guard against overflow */
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) { /* allocation failure */
            free(cache);
            exit(1);
        }
        cache = new_cache;
        cache_cap = new_cap;

        /* Compute new terms from cache_len up to n */
        for (idx_t m = (idx_t)cache_len; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;              /* C(m-1, 0) */
            idx_t m_minus1 = m - 1;
            for (idx_t i = 0; i < m; ++i) {
                if (i != 2) {
                    /* Add term binom * a[m-1-i] */
                    sum += binom * cache[(size_t)(m - 1 - i)];
                }
                /* Update binomial coefficient for next i */
                if (i < m_minus1) {
                    binom = binom * (m_minus1 - i) / (i + 1);
                }
            }
            cache[(size_t)m] = sum;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
