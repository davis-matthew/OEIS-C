/*
 * A229741  a(0)=1; thereafter a(n) = n! + Sum_{i=0..n-1} a(i)*a(n-1-i).
 * https://oeis.org/A229741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = n! + Σ_{i=0}^{n-1} a(i) * a(n-1-i)   (n ≥ 1) */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* cache of already computed terms a(0)…a(k) */
    static seq_t *cache   = NULL;   /* array of a(0)…a(k)              */
    static size_t cache_cap = 0;    /* allocated capacity              */
    static idx_t  cache_len = 0;    /* number of terms already computed */
    static seq_t  fact      = 1;    /* factorial of (cache_len-1)      */

    /* grow cache if necessary */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap < (size_t)n + 1) {
        cache_cap *= 2;
        if (cache_cap < (size_t)n + 1) cache_cap = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute terms up to n on demand */
    while (cache_len <= n) {
        idx_t idx = cache_len;

        if (idx == 0) {                /* a(0) = 1 */
            cache[0] = (seq_t)1;
            fact = (seq_t)1;           /* 0! = 1 */
        } else {
            /* update factorial: idx! = (idx-1)! * idx */
            fact *= (seq_t)idx;

            /* convolution sum Σ_{i=0}^{idx-1} a(i)*a(idx-1-i) */
            seq_t sum = (seq_t)0;
            for (idx_t i = 0; i < idx; ++i) {
                sum += cache[i] * cache[idx - 1 - i];
            }

            cache[idx] = fact + sum;
        }

        ++cache_len;
    }

    return cache[n];
}
