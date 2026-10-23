/*
 * A115971  a(0) = 0. If a(n) = 0, then a(2^n) through a(2^(n+1)-1) are each
 *          equal to 1. If a(n) = 1, then a(m + 2^n) = a(m) for each m, 0 <= m
 *          <= 2^n -1.
 * https://oeis.org/A115971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0; for n>0, let k=floor(log2(n)). If a(k)=0 then a(n)=1,
 *            else a(n)=a(n-2^k)
 * Range:     u8 n <= 9591, u16 n <= 9591, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>  /* for uint64_t in clzll cast */

seq_t a(idx_t n)
{
    /* static cache for incremental construction   */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries currently computed */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Grow cache when necessary */
    if (n >= cache_cap) {
        idx_t needed = n + 1;
        idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* out of memory */
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise the first term if not yet done */
    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* floor(log2(i)) for i>0 */
        idx_t k = 63 - __builtin_clzll((uint64_t)i);
        if (cache[k] == 0)
            cache[i] = 1;
        else
            cache[i] = cache[i - ((idx_t)1 << k)];
    }
    cache_len = n + 1;

    return cache[n];
}
