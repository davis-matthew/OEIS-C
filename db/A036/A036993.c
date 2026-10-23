/*
 * A036993  Numbers n with property that reading from right to left in the
 *          binary expansion of n, the number of 0's always stays ahead of the
 *          number of 1's.
 * https://oeis.org/A036993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) = the n-th nonnegative integer whose binary representation,
 *            read from right to left, has more 0's than 1's in every prefix.
 *            Explicitly, a(1)=0, a(2)=4, a(3)=8, …
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   good_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/good_u64.h"

static seq_t *cache = NULL;          /* stored sequence values      */
static size_t cache_len = 0;         /* number of values stored    */
static size_t cache_cap = 0;         /* allocated capacity         */
static uint64_t next_search = 1;     /* next integer to test       */
static int initialized = 0;          /* one‑time initialization    */

/* ensure that the cache can hold at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = p;
    cache_cap = new_cap;
}

/* main sequence routine ------------------------------------------ */
seq_t a(idx_t n)
{
    /* one‑time set‑up */
    if (!initialized) {
        initialized = 1;
        /* store a(1)=0 */
        ensure_cache(1);
        cache[0] = 0;
        cache_len = 1;
        next_search = 1;
    }

    /* n is 1‑based */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* compute additional terms until we have the n‑th */
    while ((size_t)cache_len < (size_t)n) {
        if (good_u64(next_search)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_search;
        }
        ++next_search;
    }

    return cache[n - 1];
}
