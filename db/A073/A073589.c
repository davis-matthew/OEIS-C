/*
 * A073589  a(n)=a(n-1)^2*2^n-1 where a(0)=1.
 * https://oeis.org/A073589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)^2 * 2^n - 1 (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A073589 iteratively, caching results for repeated or out‑of‑order calls. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache storing already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of terms stored */
    static idx_t cache_cap = 0;      /* current allocation size */
    static seq_t power2 = 1;         /* 2^k for k = cache_len */

    /* initialise cache for the first call */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_len = 1;                /* next index to compute will be 1 */
    }

    /* extend cache up to the requested index */
    while (cache_len <= n) {
        /* next 2^k */
        power2 *= (seq_t)2;

        /* a(k) = a(k-1)^2 * 2^k - 1 */
        seq_t prev = cache[cache_len - 1];
        seq_t val  = prev * prev * power2 - (seq_t)1;

        /* grow array if necessary */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }

        cache[cache_len++] = val;
    }

    return cache[n];
}
