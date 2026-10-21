/*
 * A052830  A simple grammar: sequences of rooted cycles.
 * https://oeis.org/A052830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! · Σ_{k=2..n} 1/(k-1) · a(n-k)/(n-k)! (with a(0)=1,
 *            a(1)=0)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation using incremental products; works for any seq_t */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;      /* array of a-values */
    static idx_t cache_cap = 0;      /* allocated size of cache      */
    static idx_t computed_max = 0;   /* highest index computed so far */

    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* allocate initial cache if necessary */
    if (cache_cap == 0) {
        cache_cap = 2;                             /* at least space for a[0], a[1] */
        cache = (seq_t *)realloc(NULL, sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        computed_max = 1;
    }

    /* grow cache if needed */
    if (n > (idx_t)(cache_cap - 1)) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until large enough */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* compute new values up to n */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;

            /* prod holds the product of terms (i-1)*(i)*(i-2)*... down to the needed range */
            seq_t prod = (seq_t)(i - 1) * (seq_t)i;   /* start with k = 2 */

            for (idx_t k = 2; k <= i; ++k) {
                if (k > 2) {
                    /* extend the product by the next smaller factor */
                    prod = prod * (seq_t)(i - k + 1);
                }

                /* term = a[i-k] * prod / (k-1) */
                seq_t term = cache[i - k] * prod;
                term = term / (seq_t)(k - 1);
                sum += term;
            }
            cache[i] = sum;
        }
        computed_max = n;
    }

    return cache[n];
}
