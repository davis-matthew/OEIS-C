/*
 * A075917  Fourth column of triangle A075501.
 * https://oeis.org/A075917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6^n) * S2(n+4, 4) Equivalent linear recurrence (order
 *            4): a(n) = 60*a(n-1) - 1260*a(n-2) + 10800*a(n-3) - 31104*a(n-4)
 *            with initial values: a(0) = 1, a(1) = 60, a(2) = 2340, a(3) =
 *            75600. The sequence is the fourth column of triangle A075501
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;         /* array of computed terms */
static size_t cache_cap = 0;        /* allocated capacity */
static idx_t cache_size = 0;        /* number of terms currently computed */

/* Extend the cache to at least n+1 entries, computing missing terms */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initial capacity */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    /* grow capacity if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t k = cache_size; k <= n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
        } else if (k == 1) {
            cache[1] = (seq_t)60;
        } else if (k == 2) {
            cache[2] = (seq_t)2340;
        } else if (k == 3) {
            cache[3] = (seq_t)75600;
        } else {
            /* recurrence: a(n) = 60*a(n-1) -1260*a(n-2) +10800*a(n-3) -31104*a(n-4) */
            seq_t v0 = cache[(size_t)(k-1)];
            seq_t v1 = cache[(size_t)(k-2)];
            seq_t v2 = cache[(size_t)(k-3)];
            seq_t v3 = cache[(size_t)(k-4)];

            /* compute as (60*v0 + 10800*v2) - (1260*v1 + 31104*v3) */
            seq_t temp1 = (seq_t)60 * v0 + (seq_t)10800 * v2;
            seq_t temp2 = (seq_t)1260 * v1 + (seq_t)31104 * v3;
            cache[(size_t)k] = temp1 - temp2;
        }
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
