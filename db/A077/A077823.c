/*
 * A077823  Expansion of (1-x)^(-1)/(1-3*x-2*x^2-3*x^3).
 * https://oeis.org/A077823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 3*a(n-1) + 2*a(n-2) + 3*a(n-3), with a(0)=1, a(1)=4,
 *            a(2)=15
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size */
    static idx_t cache_len = 0;   /* number of entries filled */

    /* Ensure the cache can hold index n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (n >= new_cap) new_cap <<= 1;           /* grow exponentially */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_cap);
        if (!tmp) abort();                            /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise a(0) if the cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
    }

    /* Compute any missing values up to and including n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)1;                       /* the constant term */
        if (i >= 1) val += (seq_t)3 * cache[i - 1];
        if (i >= 2) val += (seq_t)2 * cache[i - 2];
        if (i >= 3) val += (seq_t)3 * cache[i - 3];
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
