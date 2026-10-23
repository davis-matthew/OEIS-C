/*
 * A124973  a(n) = Sum_{k=0..(n-2)/2} a(k)a*(n-1-k), with a(0) = a(1) = 1.
 * https://oeis.org/A124973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..(n-2)/2} a(k) * a(n-1-k), a(0)=a(1)=1
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 33, u64 n <= 60, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Cached iterative evaluation; works for any caller order. */

#include <stddef.h>
#include <stdlib.h>

/* Static cache of already computed terms */
static seq_t *cache = NULL;           /* array a[i] */
static size_t cache_cap = 0;          /* currently allocated capacity (in entries) */
static idx_t known_last = 1;          /* highest n that has been computed */

static void
ensure_cache(idx_t n)
{
    /* allocate or grow the cache so that indices up to n are valid */
    if (cache == NULL) {
        size_t cap = (size_t)n + 1;
        if (cap < 8)          /* minimal initial size */
            cap = 8;
        /* round up to next power of two for future growth */
        while ((cap & (cap - 1)) != 0) cap <<= 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) {
            /* If allocation fails, the program cannot continue */
            abort();
        }
        /* initialise base terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_cap = cap;
        known_last = 1;
        return;
    }

    /* grow if necessary */
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t cap = cache_cap;
    while ((size_t)n + 1 > cap)
        cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (tmp == NULL) {
        abort();
    }
    cache = tmp;
    cache_cap = cap;
}

/* Public function required by the build */
seq_t
a(idx_t n)
{
    if (n == 0 || n == 1) {
        return (seq_t)1;          /* base values */
    }

    ensure_cache(n);

    /* Compute any missing terms up to n */
    for (idx_t i = known_last + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t up = (i - 2) / 2;        /* floor((i-2)/2) */
        for (idx_t k = 0; k <= up; ++k) {
            sum += cache[k] * cache[i - 1 - k];
        }
        cache[i] = sum;
    }
    known_last = n;
    return cache[n];
}
