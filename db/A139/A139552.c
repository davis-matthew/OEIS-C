/*
 * A139552  a(n) = lcm(1..floor(n/3)).
 * https://oeis.org/A139552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1..floor(n/3))
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* static cache for lcm(1..m) values, indexed by m */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* enlarge the cache array to hold at least 'needed' elements */
static void ensure_cache_capacity(idx_t needed) {
    if ((idx_t)cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                 /* unrecoverable memory error */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    /* floor division of n by 3 */
    idx_t m = n / 3;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = 1;            /* lcm of empty set is 1 */
        cache_len = 1;
    }

    /* return cached value if already computed */
    if (m < cache_len)
        return cache[m];

    /* compute lcm sequentially up to m */
    ensure_cache_capacity(m + 1);
    for (idx_t i = cache_len; i <= m; ++i) {
        seq_t prev = cache[i - 1];
        seq_t g = gcd(prev, (seq_t)i);       /* previous lcm and i */
        seq_t tmp = prev / g;                /* divide before multiply */
        seq_t new_val = tmp * (seq_t)i;       /* next lcm */
        cache[i] = new_val;
    }
    cache_len = m + 1;
    return cache[m];
}
