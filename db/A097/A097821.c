/*
 * A097821  Expansion of e.g.f. exp(2x)/(1-5x).
 * https://oeis.org/A097821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(n) = 5*n*a(n-1) + 2^n (n > 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previous terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of terms stored = cache_len */
    static seq_t pow2 = (seq_t)1;        /* 2^(cache_len-1) */

    /* initialise cache if empty */
    if (cache_len == 0)
    {
        cache = (seq_t*)calloc(1, sizeof(seq_t));
        cache[0] = (seq_t)1;            /* a(0) */
        cache_len = 1;
        pow2 = (seq_t)1;                /* 2^0 */
    }

    /* return immediately if already computed */
    if (n < cache_len)
        return cache[n];

    /* otherwise extend cache to include up to n */
    idx_t old_len = cache_len;          /* length before extension */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t*)calloc(new_len, sizeof(seq_t));
    for (idx_t j = 0; j < old_len; ++j)
        new_cache[j] = cache[j];
    free(cache);
    cache = new_cache;
    cache_len = new_len;                /* now cache_len == new_len */

    /* compute missing terms, pow2 already equals 2^(old_len-1) */
    for (idx_t i = old_len; i <= n; ++i)
    {
        pow2 *= (seq_t)2;                           /* pow2 = 2^i   */
        seq_t five_n = ((seq_t)5 * (seq_t)i);       /* 5*i          */
        seq_t term  = cache[i-1] * five_n + pow2;   /* recurrence   */
        cache[i] = term;
    }

    return cache[n];
}
