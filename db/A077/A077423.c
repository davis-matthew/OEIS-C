/*
 * A077423  Chebyshev sequence U(n,12)=S(n,24) with Diophantine property.
 * https://oeis.org/A077423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - a(n-2), with a(0)=1, a(1)=24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache to memoise previously computed terms. */
static seq_t *cache = NULL;
static idx_t cache_len   = 0;  /* number of cached terms */
static idx_t cache_cap   = 0;  /* current allocated capacity */

/* Initialise the cache with the first two terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache_cap = 16;                     /* arbitrary initial size */
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)
        abort();                /* out of memory */

    cache[0] = (seq_t)1;               /* a(0) */
    cache[1] = (seq_t)24;              /* a(1) */
    cache_len = 2;
}

/* Grow the cache so it can hold at least 'need' entries. */
static void ensure_cap(idx_t need)
{
    if (need <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;     /* double the capacity until large enough */

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();           /* out of memory */

    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of OEIS sequence A077423 (n ≥ 0). */
seq_t a(idx_t n)
{
    init_cache();

    /* Compute missing terms up to n using the recurrence. */
    if (n >= cache_len) {
        idx_t target = n + 1;            /* need indices 0 … n */
        ensure_cap(target);
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = 24*a(i-1) - a(i-2) */
            cache[i] = (seq_t)(24 * cache[i-1] - cache[i-2]);
        }
        cache_len = target;
    }

    return cache[n];
}
