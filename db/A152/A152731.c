/*
 * A152731  a(n) + a(n+1) + a(n+2) = n^6, a(1)=a(2)=0.
 * https://oeis.org/A152731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)^6 - a(n-1) - a(n-2) for n ≥ 3, a(1) = a(2) = 0. The
 *            sequence satisfies the restriction a(n)+a(n+1)+a(n+2)=n^6
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 49, u64 n <= 1953,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return base cases immediately. */
    if (n <= 2)
        return (seq_t)0;

    /* Static cache for already computed values. */
    static seq_t *cache   = NULL;   /* 1‑based indexing, cache[0] unused */
    static idx_t  cache_sz = 0;     /* number of allocated slots (>=0) */
    static idx_t  last_computed = 2;/* largest index whose value is valid */

    /* Resize cache if required. */
    if (cache_sz < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp)   /* allocation failed – give up */
            return (seq_t)0;
        /* Initialise newly allocated slots to zero. */
        for (idx_t i = cache_sz; i <= n; ++i)
            tmp[i] = (seq_t)0;
        cache   = tmp;
        cache_sz = n + 1;
    }

    /* Initialise a(1) and a(2) when the cache was empty before. */
    if (cache_sz <= 2) {
        if (cache_sz < 3) {
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)0;
        }
    }

    /* Compute missing terms up to n. */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        idx_t m = k - 2;
        seq_t m_s = (seq_t)m;
        /* (k-2)^6 computed multiplicatively to avoid large temporaries. */
        seq_t pow6 = m_s * m_s * m_s * m_s * m_s * m_s;
        cache[k] = pow6 - cache[k - 1] - cache[k - 2];
    }
    last_computed = n;

    return cache[n];
}
