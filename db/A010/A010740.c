/*
 * A010740  Shifts 3 places right under binomial transform.
 * https://oeis.org/A010740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 21, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = a(1) = a(2) = 1;
   For n >= 3:
       a(n) = a(n-3) - Σ_{k=0}^{n-1} C(n,k) * a(k)
   where C(n,k) is the binomial coefficient.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t sz = 0;   /* number of cached terms, indices [0,sz-1] */

    /* Ensure space for at least the first three terms */
    if (sz < 3) {
        idx_t new_sz = 3;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */
        cache = tmp;
        for (idx_t i = sz; i < new_sz; ++i)
            cache[i] = 1;                    /* a(0)=a(1)=a(2)=1 */
        sz = new_sz;
    }

    /* If the requested term is already computed, return it */
    if (n < sz)
        return cache[n];

    /* Extend the cache to hold terms up to n */
    if (n >= sz) {
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */
        cache = tmp;
        sz = new_sz;
    }

    /* Compute terms sequentially from the first missing one */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t binom = 1;                     /* C(i,0) */

        for (idx_t k = 0; k < i; ++k) {
            sum += binom * cache[k];
            /* Update binom = C(i,k+1) = C(i,k) * (i-k)/(k+1) */
            binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
        }

        cache[i] = cache[i - 3] - sum;
    }

    return cache[n];
}
