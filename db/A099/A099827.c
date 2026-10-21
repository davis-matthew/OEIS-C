/*
 * A099827  Generalized harmonic number H(n,5) = Sum_{k=1..n} 1/k^5 multiplied
 *          by (n!)^5.
 * https://oeis.org/A099827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^5 * Sum_{k=1..n} 1/k^5 Recurrence: a(0)=0, a(1)=1,
 *            a(n) = ((n-1)^5 + n^5) * a(n-1) - (n-1)^10 * a(n-2) for n > 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Trivial case */
    if (n == 0) return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache    = NULL;
    static size_t cache_sz = 0;
    static idx_t  last     = 0;   /* highest index computed so far */

    /* Allocate or grow cache if necessary */
    if (cache == NULL) {
        cache_sz = (size_t)(n + 1);
        cache    = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        cache[0] = (seq_t)0;
        if (cache_sz > 1) cache[1] = (seq_t)1;
        last = (cache_sz > 1) ? 1 : 0;
    } else if ((size_t)(n + 1) > cache_sz) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_sz = (size_t)(n + 1);
        /* Newly allocated elements will be filled by the loop below */
    }

    /* Compute any missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t m_t = (seq_t)(i - 1); /* (i-1) as seq_t */
        seq_t i_t = (seq_t)i;       /*  i   as seq_t */

        /* (i-1)^5 and i^5 */
        seq_t m5 = 1;
        for (int k = 0; k < 5; ++k) m5 *= m_t;

        seq_t i5 = 1;
        for (int k = 0; k < 5; ++k) i5 *= i_t;

        seq_t sum   = m5 + i5;
        seq_t left  = sum * cache[i - 1];

        seq_t m10   = m5 * m5;
        seq_t right = m10 * cache[i - 2];

        cache[i] = left - right;
    }
    last = (last > n) ? last : n;

    return cache[n];
}
