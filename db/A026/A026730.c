/*
 * A026730  a(8n)=n, a(8n+4)=a(8n)+a(8n+8), a(4n+2)=a(4n)+a(4n+4),
 *          a(2n+1)=a(2n)+a(2n+2).
 * https://oeis.org/A026730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 410, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0..15) are fixed, a(n) = 2*a(n-8) - a(n-16) for n >= 16. */
#include <stddef.h>
#include <stdlib.h>

/* Initial 16 terms from the OEIS entry */
static const seq_t init[16] = {
    0, 1, 1, 2,
    1, 3, 2, 3,
    1, 5, 4, 7,
    3, 8, 5, 7
};

/* Static cache for memoization (external across calls). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* current allocated capacity */
static size_t cache_len = 0;   /* number of elements already computed */

/* Ensure the cache has at least n elements. */
static void ensure_size(size_t n)
{
    if (cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main function: compute a(n). */
seq_t a(idx_t n)
{
    /* Fast return if already computed. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow cache and compute missing values. */
    size_t new_len = (size_t)n + 1;
    ensure_size(new_len);

    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        size_t idx = (size_t)i;
        if (i < 16) {
            cache[idx] = init[idx];
        } else {
            cache[idx] = (cache[idx - 8] << 1) - cache[idx - 16];
        }
    }
    cache_len = new_len;

    return cache[(size_t)n];
}
