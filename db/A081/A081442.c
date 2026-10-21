/*
 * A081442  Expansion of e.g.f.: cosh(x/sqrt(1-x^2)) (even powers).
 * https://oeis.org/A081442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (12*n^2 - 24*n + 13)*a(n-1) -
 *            12*(n-2)*(n-1)*(2*n-3)^2*a(n-2) +
 *            16*(n-3)*(n-2)^2*(n-1)*(2*n-5)*(2*n-3)*a(n-3) with a(0) = 1,
 *            a(1) = 1, a(2) = 13
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed values.  The array starts with the first
       three terms of the sequence and grows as needed. */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (number of slots) */
    static idx_t cache_len = 0;   /* number of terms already computed */

    /* Allocate initial cache if this is the first call. */
    if (cache == NULL) {
        cache_cap = 4;                        /* start with a small capacity */
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) abort();           /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)13;
        cache_len = 3;                        /* terms 0,1,2 are known */
    }

    /* If the requested index is already in the cache we can return it. */
    if (n < cache_len)
        return cache[n];

    /* Ensure that the cache has enough space for the new terms. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;                   /* double until it is big enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) abort();            /* reallocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term;
        if (i == 0) {
            term = (seq_t)1;
        } else if (i == 1) {
            term = (seq_t)1;
        } else if (i == 2) {
            term = (seq_t)13;
        } else {
            /* Coefficients of the recurrence. */
            seq_t c1 = (seq_t)(12ULL * i * i - 24ULL * i + 13ULL);
            seq_t c2 = (seq_t)12 *
                       (seq_t)(i - 2) *
                       (seq_t)(i - 1) *
                       (seq_t)(2 * i - 3) *
                       (seq_t)(2 * i - 3);
            seq_t c3 = (seq_t)16 *
                       (seq_t)(i - 3) *
                       (seq_t)(i - 2) *
                       (seq_t)(i - 2) *
                       (seq_t)(i - 1) *
                       (seq_t)(2 * i - 5) *
                       (seq_t)(2 * i - 3);

            /* Apply the recurrence: term = c1*a(i-1) - c2*a(i-2) + c3*a(i-3) */
            seq_t term1 = c1 * cache[i - 1];
            seq_t term2 = c2 * cache[i - 2];
            seq_t term3 = c3 * cache[i - 3];
            term = term1 - term2 + term3;
        }

        cache[i] = term;
    }

    cache_len = n + 1;          /* now all terms up to n are stored */
    return cache[n];
}
