/*
 * A174763  x-values in the solution to x^2-62*y^2=1.
 * https://oeis.org/A174763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 126*a(n-1) - a(n-2) * initial terms: a(1) = 1, a(2) = 63
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of previously computed values (1‑based indexing) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_len = 0;   /* number of initialized terms, up to cache_len */

/* enlarge the cache until it can hold n elements (indices 0..n-1) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
    while (new_cap < (size_t)n)
        new_cap *= 2;

    /* enlarge; use an explicit cast so the code compiles as C++ */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* compute the n‑th term of the sequence – undefined for n < 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* with offset 1 this case is not used */

    ensure_cache(n);

    /* initialise base cases if not already done */
    if (cache_len < 1) {
        cache[0] = (seq_t)1;          /* a(1) */
        cache_len = 1;
    }
    if (cache_len < 2) {
        cache[1] = (seq_t)63;         /* a(2) */
        cache_len = 2;
    }

    /* compute missing terms up to n, using the recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t term = (seq_t)126 * cache[i - 1] - cache[i - 2];
        cache[i] = term;
    }
    cache_len = (size_t)n;

    return cache[(size_t)n - 1];
}
