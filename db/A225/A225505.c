/*
 * A225505  a(n) = triangular(a(n-1)+a(n-2)) if n > 1, else a(n) = n.
 * https://oeis.org/A225505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n ≤ 1; a(n) = triangular(a(n-1)+a(n-2)) for n > 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed a(k) */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of entries computed */

static void ensure_cache_size(size_t n)
{
    if (n < cache_len)
        return;

    if (cache_len == 0) {                 /* initial allocation */
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
    } else if (n >= cache_cap) {          /* grow to at least n+1 */
        size_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_len; i <= n; ++i) {
        if (i == 0u) {
            cache[0] = (seq_t)0;
        } else if (i == 1u) {
            cache[1] = (seq_t)1;
        } else {
            seq_t s = cache[i-1] + cache[i-2];
            cache[i] = s * (s + (seq_t)1) / (seq_t)2;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_cache_size(idx);
    return cache[idx];
}
