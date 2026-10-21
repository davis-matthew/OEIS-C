/*
 * A090591  Expansion of g.f. 1/(1 - 2*x + 8*x^2).
 * https://oeis.org/A090591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - 8*a(n-2), a(0) = 1, a(1) = 2. A linear
 *            recurrence that can be evaluated iteratively in O(n)
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 20, i64 n <= 41, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cache of computed terms */
static size_t cache_cap = 0;       /* current capacity of cache */
static idx_t cache_len = 0;        /* number of terms currently cached */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* If allocation fails, exit with abort to avoid memory corruption. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) using recurrence, memoizing results. */
seq_t a(idx_t n)
{
    /* Initialize first two terms if cache empty */
    if (cache_len == 0) {
        ensure_cache_cap(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* If requested term already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Prepare to compute terms from current cache_len up to n */
    idx_t i;
    seq_t prev2 = cache[cache_len - 2];
    seq_t prev1 = cache[cache_len - 1];

    /* Expand cache if necessary */
    ensure_cache_cap((size_t)(n + 1));

    for (i = cache_len; i <= n; ++i) {
        seq_t curr = 2 * prev1 - 8 * prev2;
        cache[i] = curr;
        prev2 = prev1;
        prev1 = curr;
    }

    cache_len = n + 1;
    return cache[n];
}
