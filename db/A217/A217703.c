/*
 * A217703  a(0)=1, a(1)=0, and a(n+1) = 2*n*(n+1)*a(n)-n^4*a(n-1) for n>0.
 * https://oeis.org/A217703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=0, a(n+1) = 2*n*(n+1)*a(n)-n^4*a(n-1) for n>0
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache      = NULL;
    static idx_t  max_cached = (idx_t)-1;   /* highest index cached so far */
    static size_t capacity   = 0;           /* allocated capacity */

    /* Allocate and initialise cache if needed */
    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        max_cached = 1;
    }

    /* Ensure cache can hold index `n` */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity;
        while ((size_t)n >= new_cap) new_cap <<= 1; /* grow by factor 2 */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute terms up to index `n` if not already cached */
    if (n > max_cached) {
        for (idx_t m = max_cached + 1; m <= n; ++m) {
            seq_t nm1 = (seq_t)(m-1);                /* m-1 as seq_t */

            /* term1 = 2*(m-1)*m*a[m-1] */
            seq_t term1 = ((seq_t)2) * nm1 * (seq_t)m * cache[m-1];

            /* n4 = (m-1)^4 */
            seq_t n4 = nm1 * nm1;   /* (m-1)^2 */
            n4 *= n4;               /* (m-1)^4 */

            /* term2 = (m-1)^4 * a[m-2] */
            seq_t term2 = n4 * cache[m-2];

            cache[m] = term1 - term2;
        }
        max_cached = n;
    }

    return cache[n];
}
