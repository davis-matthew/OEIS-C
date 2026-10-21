/*
 * A239195  Sum of the next to smallest parts in the partitions of 4n into 4
 *          parts with smallest part = 1.
 * https://oeis.org/A239195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the recurrence a(n) =
 *            2*a(n-1)-a(n-2)+2*a(n-3)-4*a(n-4)+2*a(n-5)-a(n-6)+2*a(n-7)-a(n-8)
 *            with initial values a(1)=1, a(2)=5, a(3)=17, a(4)=42, a(5)=78,
 *            a(6)=134, a(7)=215, a(8)=315
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_len = 0;

/* ensure the allocated cache is large enough for indices 0..n inclusive */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;          /* 1‑based indexing, ignore index 0 */
    if (need <= cache_cap) return;
    seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = need;
}

/* compute terms up to n, storing them in cache */
static void ensure_upto(idx_t n)
{
    if (n <= cache_len) return;
    ensure_cache(n);

    static const seq_t init[8] = {1,5,17,42,78,134,215,315};

    /* fill initial values up to min(8, n) */
    for (idx_t i = cache_len + 1; i <= n && i <= 8; ++i)
        cache[i] = init[i-1];

    /* recurrence for i >= 9 */
    idx_t start = (cache_len >= 8) ? cache_len + 1 : 9;
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t v = (seq_t)2 * cache[i-1];
        v -= cache[i-2];
        v += (seq_t)2 * cache[i-3];
        v -= (seq_t)4 * cache[i-4];
        v += (seq_t)2 * cache[i-5];
        v -= cache[i-6];
        v += (seq_t)2 * cache[i-7];
        v -= cache[i-8];
        cache[i] = v;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* sequence is defined for n ≥ 1, but guard anyway */
    ensure_upto(n);
    return cache[n];
}
