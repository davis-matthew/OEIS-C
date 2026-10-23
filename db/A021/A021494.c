/*
 * A021494  Expansion of 1/((1-x)(1-3x)(1-6x)(1-7x)).
 * https://oeis.org/A021494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 97*a(n-2) + 207*a(n-3) - 126*a(n-4) (n > 3)
 *            a(0) = 1, a(1) = 17, a(2) = 192, a(3) = 1822 Uses an expanding
 *            cache so that calls in any order are answered correctly
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache of already-computed terms */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_cap = 0;         /* allocated capacity (in terms) */
static size_t cache_len = 0;         /* number of terms that are initialized */

/* Ensure the cache has room for at least (max_n + 1) terms.  Does NOT
   change cache_len. */
static void ensure_cache(idx_t max_n)
{
    size_t needed = (size_t)max_n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                    /* allocation failed */

    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Install the first four terms if not already done. */
    if (cache_len < 4) {
        ensure_cache(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        cache[2] = (seq_t)192;
        cache[3] = (seq_t)1822;
        cache_len = 4;
    }

    /* If the requested index has already been computed, return it. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Make sure we have room for all needed terms. */
    ensure_cache(n);

    /* Compute missing terms from cache_len up to n inclusive. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 17*a(i-1) - 97*a(i-2) + 207*a(i-3) - 126*a(i-4)  (i > 3) */
        seq_t term_pos = cache[i-1] * (seq_t)17 + cache[i-3] * (seq_t)207;
        seq_t term_neg = cache[i-2] * (seq_t)97  + cache[i-4] * (seq_t)126;
        cache[i] = term_pos - term_neg;
    }

    /* Update cache_len to the highest indexed term that is now available. */
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
