/*
 * A096535  a(0) = a(1) = 1; a(n) = (a(n-1) + a(n-2)) mod n.
 * https://oeis.org/A096535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(0) = a(1) = 1; a(n) = (a(n-1) + a(n-2)) mod n (n ≥ 2)
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* initialise the first two terms */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache_size = 2;
    }

    if (n < cache_size) return cache[n];

    /* grow and fill the cache up to n */
    if (cache_size <= n) {
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;
        cache = new_cache;
        for (idx_t k = cache_size; k <= n; ++k) {
            seq_t prev1 = cache[k-1];
            seq_t prev2 = cache[k-2];
            seq_t sum  = prev1 + prev2;
            cache[k]   = sum % (seq_t)k;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
