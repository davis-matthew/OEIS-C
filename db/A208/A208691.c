/*
 * A208691  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A208691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 17*a(n-2) + 4*a(n-3) - 32*a(n-4) Initial values:
 *            a(1)=14, a(2)=196, a(3)=490, a(4)=3430
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache can hold at least n elements (1‑based indices) */
static void ensure_cache_size(idx_t n)
{
    if (n <= (idx_t)cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 4 : cache_cap;
    while ((idx_t)new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure – will crash on access */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* offset is 1, undefined below 1 */

    /* initialize cache for first four terms if not yet done */
    if (cache_len == 0) {
        ensure_cache_size(4);
        cache[0] = (seq_t)14;
        cache[1] = (seq_t)196;
        cache[2] = (seq_t)490;
        cache[3] = (seq_t)3430;
        cache_len = 4;
    }

    /* already computed? */
    if (n <= cache_len)
        return cache[n - 1];

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* retrieve previous four terms */
        seq_t a1 = cache[i - 2];   /* a(i-1) */
        seq_t a2 = cache[i - 3];   /* a(i-2) */
        seq_t a3 = cache[i - 4];   /* a(i-3) */
        seq_t a4 = cache[i - 5];   /* a(i-4) */

        seq_t val = a1 + (seq_t)17 * a2 + (seq_t)4 * a3 - (seq_t)32 * a4;

        ensure_cache_size(i);
        cache[i - 1] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
