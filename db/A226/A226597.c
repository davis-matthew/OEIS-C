/*
 * A226597  a(n) = c(1,2,...,n), the Cantor tuple function c applied to
 *          n-tuple (1,2,...,n).
 * https://oeis.org/A226597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+n)*(a(n-1)+n+1)/2 + n (for n>1); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache    = NULL;
    static idx_t cache_len = 0;  /* number of initialized entries in cache */
    static size_t cache_cap = 0; /* allocated capacity */

    /* initialise cache for index 0 if necessary */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory, return 0 */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* grow cache as needed */
    while (cache_len <= n) {
        idx_t k = cache_len;  /* index to compute now */

        seq_t val;
        if (k <= (idx_t)1) {
            val = (seq_t)k;  /* a(0)=0, a(1)=1 */
        } else {
            seq_t a_prev = cache[k - 1];
            seq_t sum = a_prev + (seq_t)k;
            val = sum * (sum + 1) / 2 + (seq_t)k;
        }

        /* ensure capacity */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;  /* allocation failed */
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n];
}
