/*
 * A099741  a(1) = a(2) = 1; a(n) = a([n/2])+a([n/3]) (n >= 3).
 * https://oeis.org/A099741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1943, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=1; a(n)=a(floor(n/2))+a(floor(n/3)) for n≥3 */

/* Note: the array `cache` is 1‑based; cache[0] is unused. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache of already‑computed terms */
static size_t cache_len = 0;  /* allocated array length   */
static size_t computed  = 0;  /* highest index that has been computed */

seq_t a(idx_t n)
{
    if (n == 0)                /* the sequence is defined for n≥1 */
        return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t init = 16;
        if (init <= (size_t)n) init = n + 1;     /* make sure large enough for the request */
        cache = (seq_t*)realloc(NULL, init * sizeof(seq_t));
        cache_len = init;
        cache[0] = (seq_t)0;                    /* unused entry */
        cache[1] = (seq_t)1;
        if (cache_len > 2) cache[2] = (seq_t)1;
        computed = 2;                            /* we have values for 1 and 2 */
    }

    /* grow the allocated array if necessary */
    if ((size_t)n >= cache_len) {
        size_t old = cache_len;
        size_t ne = old ? old << 1 : 16;
        while (ne <= (size_t)n) ne <<= 1;
        cache = (seq_t*)realloc(cache, ne * sizeof(seq_t));
        cache_len = ne;
    }

    /* compute any missing terms up to n */
    if ((size_t)n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            cache[i] = cache[i / 2] + cache[i / 3];
        }
        computed = (size_t)n;
    }

    return cache[n];
}
