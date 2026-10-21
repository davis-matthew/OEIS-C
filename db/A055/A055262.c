/*
 * A055262  n + sum of digits of a(n-1).
 * https://oeis.org/A055262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n + digit_sum(a(n-1)), a(0) = 0
 * Range:     u8 n <= 245, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Assume the build supplies a digit_sum function for seq_t */
#include "../utils/digit_sum.h"

/* static cache state ----------------------------------------------------- */
static seq_t *cache = NULL;      /* array of terms */
static size_t cache_len = 0;     /* number of valid terms in cache */
static size_t cache_cap = 0;     /* allocated capacity */

/* enlarge the cache if necessary ---------------------------------------- */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* main sequence routine -------------------------------------------------- */
seq_t a(idx_t n)
{
    /* return already cached value if available */
    if (n < cache_len) return cache[n];

    /* make sure we have space for the requested index (plus 1) */
    ensure_cache_cap((size_t)n + 1);

    /* initialise base case if the cache is empty */
    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t sum   = digit_sum(prev);
        cache[i]    = (seq_t)i + sum;      /* cast i to seq_t for exactness */
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
