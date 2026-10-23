/*
 * A033815  Number of standard permutations of [ a_1..a_n b_1..b_n ] (b_i is
 *          not immediately followed by a_i, for all i).
 * https://oeis.org/A033815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*n*(2*n-1)*a(n-1) + n*(n-1)*a(n-2), with a(0)=a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* memoised recurrence up to the largest n requested so far */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of pre‑computed elements */

    /* If we already have the value, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* Ensure base cases a(0)=a(1)=1 are stored */
    if (cache_size < 2) {
        seq_t *tmp = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        /* old_size will be set below as cache_size after this step */
    }

    /* Remember the size before we possibly reallocate */
    idx_t old_size = cache_size;
    if (cache_size < 2)
        old_size = 2;               /* the base cases use indices 0 and 1 */

    /* Extend storage if n is larger than what we have */
    if ((size_t)(n + 1) > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = n + 1;
    }

    /* Compute new terms from the first missing index up to n */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t twopi   = (seq_t)i * (seq_t)2;
        seq_t factor1 = twopi * (twopi - (seq_t)1);       /* 2i*(2i-1)   */
        seq_t factor2 = (seq_t)i * ((seq_t)i - (seq_t)1);  /* i*(i-1)     */
        cache[i] = factor1 * cache[i - 1] + factor2 * cache[i - 2];
    }

    return cache[n];
}
