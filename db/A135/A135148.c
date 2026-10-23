/*
 * A135148  A binomial recursion: a(n) = q(n) (see formula).
 * https://oeis.org/A135148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0, a(n) = 1 + sum_{k=1}^{n-1} (2 + C(n,k)) * a(k) for n
 *            >= 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array indexed from 1 */
    static idx_t cache_cap = 0;   /* current allocated size */
    static idx_t cached_max = 0;  /* largest n computed so far */

    if (n < 1) return (seq_t)0;   /* outside the offset, value is irrelevant */

    /* grow the cache if necessary */
    if (cache_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory */
        cache = tmp;
        cache_cap = n + 1;
    }

    /* initialise the first value */
    if (cached_max < 1) {
        cache[1] = (seq_t)0;
        cached_max = 1;
    }

    /* compute any missing values up to n */
    for (idx_t cur = cached_max + 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)1;          /* the leading “1” in the recurrence */
        seq_t binom = (seq_t)1;        /* C(cur,0) */

        /* iterate over k = 1 .. cur-1 */
        for (idx_t k = 1; k < cur; ++k) {
            /* update binom to C(cur,k) */
            binom = binom * (seq_t)(cur - k + 1) / (seq_t)k;

            /* term = (binom + 2) * a(k) */
            seq_t term = (binom + (seq_t)2) * cache[k];

            sum = sum + term;
        }

        cache[cur] = sum;
    }
    cached_max = n;
    return cache[n];
}
