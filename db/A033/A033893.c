/*
 * A033893  Sort then Add, a(1)=3.
 * https://oeis.org/A033893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3; a(n+1) = a(n) + sort_ascending(a(n))
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 70, u64 n <= 306,
 *            u128 n <= 3578, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Construct the number formed by the digits of x sorted non‑decreasingly */
static seq_t sort_ascending(seq_t x)
{
    unsigned int freq[10] = {0};

    /* Count digits of x */
    if (x == 0) {
        freq[0] = 1;
    } else {
        while (x > 0) {
            unsigned int d = (unsigned int)(x % 10);
            freq[d]++;
            x /= 10;
        }
    }

    /* Build the result */
    seq_t res = 0;
    for (int d = 0; d < 10; ++d) {
        for (unsigned int k = 0; k < freq[d]; ++k) {
            res = res * 10 + (seq_t)d;
        }
    }
    return res;
}

/* Ensure that cache[0 .. n-1] are computed (for n >= 1) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;   /* already up to date */

    /* Grow capacity: double until enough, but never below 16 */
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    if (new_cap < 16)
        new_cap = 16;

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else if (new_cap > cache_cap) {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache_cap = new_cap;

    /* Compute terms from cache_len up to n-1 */
    for (size_t idx = cache_len; idx < (size_t)n; ++idx) {
        if (idx == 0) {                     /* a(1) */
            cache[idx] = (seq_t)3;
        } else {
            seq_t prev   = cache[idx - 1];  /* a(idx+1) uses a(idx) */
            seq_t sorted = sort_ascending(prev);
            cache[idx] = prev + sorted;
        }
        cache_len = idx + 1;
    }
}

/* Public function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n=0, but safe return */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
