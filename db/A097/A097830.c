/*
 * A097830  Partial sums of Chebyshev sequence S(n,16) = U(n,16/2) =
 *          A077412(n).
 * https://oeis.org/A097830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - a(n-2) + 1 with a(0)=1, a(1)=17
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic cache for already computed terms */
static size_t cache_len = 0;  /* number of entries currently stored (cache_len-1 = max n cached) */

static void ensure_cache(idx_t n)
{
    size_t needed_len = (size_t)n + 1;
    if (needed_len <= cache_len) return;

    /* grow the cache to at least needed_len (doubling for speed) */
    size_t new_len = cache_len ? cache_len : 1;
    while (new_len < needed_len) new_len <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* initialise base cases if they have not been set yet */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;        /* a(0) */
        cache_len = 1;
        if (needed_len < 2) return; /* only a(0) needed */
    }

    if (cache_len == 1 && needed_len >= 2) {
        cache[1] = (seq_t)17;       /* a(1) */
        cache_len = 2;
        if (needed_len < 3) return; /* only a(1) needed */
    }

    /* compute remaining terms using the recurrence */
    for (size_t i = cache_len; i < needed_len; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)((seq_t)16 * prev - prev2 + 1);
    }
    cache_len = needed_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
