/*
 * A218722  a(n) = (19^n-1)/18.
 * https://oeis.org/A218722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (19^n - 1)/18 Recurrence: a(n) = 20*a(n-1) - 19*a(n-2),
 *            a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* allocated capacity */
static idx_t cache_len = 0;          /* number of calculated entries */

/* Grow the cache to accommodate at least need entries */
static void grow_cache(idx_t need)
{
    if ((size_t)need < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) using memoised recurrence */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold n+1 entries */
    grow_cache(n + 1);

    /* If already computed, return immediately */
    if (cache_len > n)
        return cache[n];

    /* Fill missing entries up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            cache[i] = (seq_t)20 * cache[i - 1] - (seq_t)19 * cache[i - 2];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
