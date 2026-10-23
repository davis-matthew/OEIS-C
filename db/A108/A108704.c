/*
 * A108704  Number of partitions of 112233...nn into n pairs.
 * https://oeis.org/A108704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = (2*n-1)*a(n-1) +
 *            (n-1)*a(n-2) - 2*(n-1)*(n-2)*a(n-3) (n≥3) with initial values
 *            a(0)=1, a(1)=1, a(2)=4. This recurrence is equivalent to the
 *            exponential generating function of the sequence
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;         /* array of computed a(i) */
    static size_t cache_cap = 0;        /* current allocated capacity */
    static idx_t  cache_len = 0;        /* number of elements actually stored (>= 3 after init) */

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache_len = 3;
    }

    /* if requested n is already cached, just return it */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) { cache[0] = (seq_t)1; continue; }
        if (i == 1) { cache[1] = (seq_t)1; continue; }
        if (i == 2) { cache[2] = (seq_t)4; continue; }

        /* Use different paths depending on whether seq_t fits in 64 bits.
           For 64‑bit (or smaller) types we use 128‑bit intermediate values
           to avoid overflow.  For arbitrary‑precision types we rely on
           operator overloading provided by the build. */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 term1 = (unsigned __int128)(2*i - 1) *
                                      (unsigned __int128)cache[i-1];
            unsigned __int128 term2 = (unsigned __int128)(i - 1) *
                                      (unsigned __int128)cache[i-2];
            unsigned __int128 term3 = (unsigned __int128)2 *
                                      (unsigned __int128)(i - 1) *
                                      (unsigned __int128)(i - 2) *
                                      (unsigned __int128)cache[i-3];
            unsigned __int128 res = term1 + term2 - term3;
            cache[i] = (seq_t)res;
        } else {
            seq_t term1 = (seq_t)(2*i - 1) * cache[i-1];
            seq_t term2 = (seq_t)(i - 1) * cache[i-2];
            seq_t term3 = ((seq_t)2 * (seq_t)(i - 1) * (seq_t)(i - 2)) * cache[i-3];
            cache[i] = term1 + term2 - term3;
        }
    }
    cache_len = n + 1;          /* we now have values up to n stored */
    return cache[n];
}
