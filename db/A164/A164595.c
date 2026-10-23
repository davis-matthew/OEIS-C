/*
 * A164595  a(n) = 10*a(n-1) - 17*a(n-2) for n > 1; a(0) = 5, a(1) = 37.
 * https://oeis.org/A164595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 17*a(n-2), a(0)=5, a(1)=37
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure the cache can hold at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = (cache_cap == 0) ? 2 : cache_cap;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return cached value if available */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to accommodate up to n */
    ensure_capacity(n + 1);

    /* Initialize base cases if the cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)5;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)37;
        cache_len = 2;
    }

    /* Compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)10 * cache[i - 1] - (seq_t)17 * cache[i - 2];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
