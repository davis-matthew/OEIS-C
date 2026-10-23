/*
 * A182182  a(0) = 0, a(1) = 1; for n > 1, a(n) = a(n-1) XOR a(n-2) XOR n.
 * https://oeis.org/A182182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(0) = 0, a(1) = 1; for n > 1, a(n) = a(n-1) XOR a(n-2) XOR n
 * Range:     u8 n <= 255, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values. */
    static seq_t *cache   = NULL;   /* array of seq_t */
    static idx_t cache_len = 0UL;   /* number of elements currently stored */

    /* Initialise cache with a(0)=0, a(1)=1 on first use. */
    if (cache_len == 0UL) {
        cache_len = 2UL;
        cache = (seq_t*)malloc(sizeof(seq_t) * (size_t)cache_len);
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* If the requested index is already cached, return it directly. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow the cache to accommodate index n. */
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (size_t)(n + 1UL));
    if (!tmp) {
        return (seq_t)0;   /* allocation failed */
    }
    cache = tmp;

    /* Compute all new terms up to n inclusive. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] ^ cache[i - 2] ^ (seq_t)i;
    }

    cache_len = n + 1UL;
    return cache[n];
}
