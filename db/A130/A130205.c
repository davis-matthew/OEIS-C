/*
 * A130205  a(n) = n^2 - a(n-1) - a(n-2), with a(1) = 1 and a(2) = 2.
 * https://oeis.org/A130205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n)=n^2-a(n-1)-a(n-2)
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* 1‑based cache for computed terms   */
static idx_t cache_cap = 0;     /* current allocated size (in elements) */
static idx_t cached_n = 0;      /* largest index already computed      */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* unused, but defines behaviour <1 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Ensure the cache can hold indices up to n */
    if (cache == NULL) {
        cache = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
        cached_n  = 0;
    } else if ((size_t)(n + 1) > (size_t)cache_cap) {
        cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
    }

    /* Initialise base values if they are not yet stored */
    if (cached_n < 1) {
        cache[1] = (seq_t)1;
        cached_n = 1;
    }
    if (cached_n < 2) {
        cache[2] = (seq_t)2;
        cached_n = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        idx_t ii   = i;                     /* use idx_t for safety      */
        idx_t sq   = ii * ii;                /* i^2 as an idx_t (no overflow) */
        idx_t sub  = (idx_t)cache[i-1] + (idx_t)cache[i-2];
        idx_t val  = sq - sub;               /* guaranteed non‑negative   */
        cache[i] = (seq_t)val;
    }
    cached_n = n;

    return cache[n];
}
