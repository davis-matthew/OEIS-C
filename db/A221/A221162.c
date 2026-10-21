/*
 * A221162  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some horizontal, vertical or antidiagonal neighbor.
 * https://oeis.org/A221162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 26*a(n-2) + a(n-3) with a(1)=3, a(2)=33,
 *            a(3)=352
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* cache[0] holds a(1) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* ensure cache holds at least n terms (n>=1) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* Grow the cache to at least n elements.  Double the capacity
       each time for amortised linear growth. */
    if (cache == NULL) {
        cache_cap = (size_t)n;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)n > cache_cap) {
        size_t newcap = cache_cap * 2;
        if (newcap < (size_t)n)
            newcap = (size_t)n;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = 3;                /* a(1) */
        } else if (i == 1) {
            cache[1] = 33;               /* a(2) */
        } else if (i == 2) {
            cache[2] = 352;              /* a(3) */
        } else {
            /* a(n) = 13*a(n-1) - 26*a(n-2) + a(n-3)   (n = i+1) */
            cache[i] = 13 * cache[i-1] - 26 * cache[i-2] + cache[i-3];
        }
    }
    cache_len = (size_t)n;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for 0, return 0 for safety */
        return 0;
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
