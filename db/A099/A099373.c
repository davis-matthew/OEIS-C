/*
 * A099373  Twice Chebyshev polynomials of the first kind, T(n,x), evaluated
 *          at 83/2.
 * https://oeis.org/A099373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 83*a(n-1) - a(n-2), a(0) = 2, a(1) = 83 (a(-1)=83 for the
 *            recurrence)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that cache contains terms a(0) … a(n) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate space for all needed terms */
        idx_t newlen = n + 1;
        cache = (seq_t *)calloc(newlen, sizeof(seq_t));
        if (!cache) return; /* allocation failure – unlikely in tests */

        /* Compute terms up to n */
        cache[0] = 2;
        if (newlen > 1) cache[1] = 83;
        for (idx_t i = 2; i <= n; ++i) {
            cache[i] = 83 * cache[i - 1] - cache[i - 2];
        }
        cache_len = newlen;
        return;
    }

    if (cache_len > n) return;   /* already computed */

    /* Grow the cache to hold at least n+1 terms */
    idx_t newlen = n + 1;
    seq_t *newptr = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    if (!newptr) return; /* allocation failure – unlikely */
    cache = newptr;

    /* Compute any newly added terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 2;
        } else if (i == 1) {
            cache[1] = 83;
        } else {
            cache[i] = 83 * cache[i - 1] - cache[i - 2];
        }
    }
    cache_len = newlen;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)-1) return 83;  /* handle the special a(-1) case if ever queried */
    ensure_cache(n);
    return cache[n];
}
