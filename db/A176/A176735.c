/*
 * A176735  a(n) = (n+8)*a(n-1) + (n-1)*a(n-2), a(-1)=0, a(0)=1.
 * https://oeis.org/A176735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+8)*a(n-1) + (n-1)*a(n-2), a(-1)=0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed a(i) */
static idx_t  cache_len  = 0; /* number of computed elements    */
static idx_t  cache_cap  = 0; /* current capacity of the array   */

/* ensure the cache can hold at least min_cap elements */
static void grow_cache(idx_t min_cap)
{
    if (min_cap <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();        /* fatal if allocation fails   */
    cache = tmp;
    cache_cap = new_cap;
}

/* sequence A176735 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* initialise cache on first call */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    if (n < cache_len) return cache[n];

    /* grow the cache to hold the new terms */
    grow_cache(n + 1);

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 1) {
            cache[i] = ((seq_t)(i + 8)) * cache[0];  /* a(-1)=0 */
        } else {
            cache[i] = ((seq_t)(i + 8)) * cache[i-1]
                       + ((seq_t)(i - 1)) * cache[i-2];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
