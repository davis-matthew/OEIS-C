/*
 * A236428  a(n) = F(n+1)^2 + F(n+1)*F(n) - F(n)^2, where F = A000045.
 * https://oeis.org/A236428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) Initial values: a(0)=1,
 *            a(1)=1, a(2)=5 (Also a(n) = F(n+1)^2 + F(n+1)*F(n) - F(n)^2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* memoisation array */
static size_t cache_cap = 0;
static idx_t max_cached = (idx_t)-1;   /* -1 means nothing cached yet */

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        if (cache_cap > 1) cache[1] = (seq_t)1;
        if (cache_cap > 2) cache[2] = (seq_t)5;
        max_cached = (cache_cap > 2 ? (idx_t)2 : (idx_t)-1);
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values that are not yet cached. */
    idx_t start = (max_cached == (idx_t)-1) ? (idx_t)3 : max_cached + 1;
    if (n >= start) {
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)2 * cache[i - 2] - cache[i - 3];
        }
        max_cached = n;
    }
}

/* OEIS A236428 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    ensure_cache(n);
    return cache[n];
}
