/*
 * A171069  G.f. -x*(x-1)*(1+x)/(1-x-12*x^2-x^3+x^4).
 * https://oeis.org/A171069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 12*a(n-2) + a(n-3) - a(n-4) Initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=12
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Empty cache: allocate and initialise the first four terms. */
    static seq_t *cache       = NULL;
    static idx_t  cache_size  = 0;   /* allocated capacity (number of slots)   */
    static idx_t  last_computed = 0; /* highest index that has been filled yet */

    /* Initialise the cache if it does not yet exist. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed – unlikely */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)12;

        cache_size     = 4;
        last_computed  = 3;
    }

    /* The first four terms are handled directly. */
    if (n < 4) {
        return cache[(idx_t)n];
    }

    /* Grow the cache so that it can hold index n. */
    if (n >= cache_size) {
        /* Grow to at least n+1, doubling to maintain amortised O(1). */
        idx_t new_size = n + 1;
        if (new_size < cache_size * 2) {
            new_size = cache_size * 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed – unlikely */
        cache      = tmp;
        cache_size = new_size;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* We know i >= 4 here, so all indices are valid. */
        seq_t val = cache[i - 1]
                    + ((seq_t)12 * cache[i - 2])
                    + cache[i - 3]
                    - cache[i - 4];
        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
