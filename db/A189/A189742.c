/*
 * A189742  a(1)=4, a(2)=3, a(n) = 4*a(n-1) + 3*a(n-2).
 * https://oeis.org/A189742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=4, a(2)=3, a(n)=4*a(n-1)+3*a(n-2) for n≥3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation caches computed values in a dynamic array so that
    repeated calls with arbitrary n work in O(n) time, where n is the
    largest value requested so far.  The cache grows only when needed. */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* uint64_t */

static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of terms currently cached */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)0;          /* undefined by OEIS, safe fallback */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)3;

    /* Ensure cache holds at least the first two values */
    if (cache_len < 2) {
        size_t initial = 2;
        cache = (seq_t *)malloc(initial * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)4;   /* a(1) */
        cache[1] = (seq_t)3;   /* a(2) */
        cache_len = initial;
    }

    /* Grow cache and compute new terms if necessary */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        /* compute terms from cache_len (already computed) up to n */
        for (size_t i = cache_len; i < new_len; ++i) {
            /* i corresponds to a(i+1) because cache is 0‑based */
            cache[i] = 4 * cache[i-1] + 3 * cache[i-2];
        }
        cache_len = new_len;
    }

    return cache[n-1];   /* cache[0] holds a(1) */
}
