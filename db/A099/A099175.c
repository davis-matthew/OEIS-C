/*
 * A099175  a(1) = 1, a(2) = 8; for n > 2: a(n) = a(n-1)*(3*a(n-1)-2).
 * https://oeis.org/A099175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*(3*a(n-1)-2), a(1)=1, a(2)=8
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* cached values a(1), a(2), ... */
static idx_t cache_len = 0;     /* number of cached terms */
static idx_t cache_cap = 0;     /* allocated capacity */

/* Ensure the cache has room for at least 'needed' terms */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < needed)
        new_cap <<= 1;          /* double until big enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the n‑th term of A099175 */
seq_t a(idx_t n)
{
    /* n is always >= 1 (offset is 1) */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)8;

    /* initialise cache if empty */
    if (cache_len == 0) {
        ensure_cache_cap(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache_len = 2;
    }

    /* if already cached, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* compute new terms up to n, extending the cache */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t last = cache[cache_len - 1];
        seq_t term = last * ((seq_t)3 * last - (seq_t)2);
        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = term;
        ++cache_len;
    }

    return cache[n - 1];
}
