/*
 * A209789  Half the number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock
 *          having exactly one duplicate clockwise edge difference.
 * https://oeis.org/A209789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 12*a(n-2), a(1)=30, a(2)=204
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache to allow out‑of‑order calls and fast O(n) computation */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* highest index computed so far */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure the cache can hold indices up to need-1 (0‑based) */
static void enlarge_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) abort();                /* fatal if allocation fails */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, return 0 */

    size_t sz = (size_t)n + 1;    /* need indices [1..n] */
    enlarge_cache(sz);

    /* Initialise base values if not already present */
    if (cache_len < 1) {
        cache[1] = (seq_t)30;
        cache_len = 1;
    }
    if (cache_len < 2 && n >= 2) {
        cache[2] = (seq_t)204;
        cache_len = 2;
    }

    /* Compute any missing terms up to n */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        cache[i] = 5 * cache[i-1] + 12 * cache[i-2];
    }
    cache_len = (size_t)n;

    return cache[(size_t)n];
}
