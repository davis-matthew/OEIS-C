/*
 * A228207  x-values in the solution to x^2 - 20*y^2 = 176.
 * https://oeis.org/A228207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-4) with a(1) = 14, a(2) = 16, a(3) = 26,
 *            a(4) = 34
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache   = NULL;  /* 1‑based cache of computed terms */
static size_t cache_cap = 0;   /* capacity of cache (in elements) */
static idx_t  ready_n   = 0;   /* largest n for which cache[n] is valid */

/* ensure that cache has room for indices 0..n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        if (cache_cap < 16) cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* return the nth term of A228207 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0 */

    /* if already computed, return it */
    if (n <= ready_n) return cache[n];

    /* grow cache to accommodate n */
    ensure_cache(n);

    /* initialise the first four terms if not yet done */
    if (ready_n < 1) cache[1] = (seq_t)14;
    if (ready_n < 2) cache[2] = (seq_t)16;
    if (ready_n < 3) cache[3] = (seq_t)26;
    if (ready_n < 4) cache[4] = (seq_t)34;

    if (ready_n < 4) ready_n = 4;

    /* compute further terms up to n */
    for (idx_t i = ready_n + 1; i <= n; ++i) {
        if (i <= 4) continue;                /* already set */
        cache[i] = (seq_t)(((seq_t)3 * cache[i - 2]) - cache[i - 4]);
    }

    ready_n = n;
    return cache[n];
}
