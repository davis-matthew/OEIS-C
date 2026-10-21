/*
 * A206209  Number of (n+1) X 3 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two equal edges, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A206209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + 4*a(n-3) - 10*a(n-4) for n>6 Initial
 *            values: a(1)=45, a(2)=130, a(3)=336, a(4)=992, a(5)=2996,
 *            a(6)=9072 Recurrence uses 0‑based caching with cache[0] unused
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* current size of the cache (index+1) */

static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* initial cache size: positions 0 … 6  (index 0 unused) */
    size_t init_len = 7;
    cache = (seq_t *)malloc(init_len * sizeof(seq_t));
    if (cache == NULL)         /* unlikely but keep the compiler happy */
        return;

    cache[0] = (seq_t)0;
    cache[1] = (seq_t)45;
    cache[2] = (seq_t)130;
    cache[3] = (seq_t)336;
    cache[4] = (seq_t)992;
    cache[5] = (seq_t)2996;
    cache[6] = (seq_t)9072;

    cache_len = init_len;
}

static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_len)
        return;

    /* grow cache to hold indices 0 … n (inclusive) */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)
        return;   /* out of memory – caller will get garbage; unlikely */

    cache = tmp;
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        cache[i] = a1 + 6 * a2 + 4 * a3 - 10 * a4;
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 1)                 /* offset is 1; behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* initialise the static cache on first call */
    init_cache();

    /* grow the cache and compute any missing values */
    ensure_cache(n);

    return cache[n];
}
