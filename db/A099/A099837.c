/*
 * A099837  Expansion of (1 - x^2) / (1 + x + x^2) in powers of x.
 * https://oeis.org/A099837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=-1, a(2)=-1, a(n) = -a(n-1)-a(n-2) for n>=3
 *            (recurrence from (1-x^2)/(1+x+x^2))
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* fallback if allocation fails */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-1;
    }

    /* if n already computed, just return */
    if (n < cache_len)
        return cache[n];

    /* extend cache up to n, computing terms with the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = -(cache[i-1] + cache[i-2]);  /* a(i) = -a(i-1)-a(i-2) */
        if (i == cache_len) {
            /* grow cache one element at a time */
            idx_t new_len = i + 1;
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* allocation failure */
            cache = tmp;
            cache_len = new_len;
        }
        /* write the newly computed value */
        cache[i] = val;
    }

    return cache[n];
}
