/*
 * A154627  Expansion of (1-5*x)/(1-8*x+4*x^2).
 * https://oeis.org/A154627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 4*a(n-2) (n>1) with a(0)=1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;        /* cached sequence values     */
static size_t cache_cap = 0;       /* allocated size (in seq_t)   */
static size_t cache_size = 0;      /* number of values that are valid (highest index+1) */

/* Expand the cache to hold at least (n+1) entries. */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;

    /* Grow factor to avoid reallocating each time. */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
    if (new_cap == 0) new_cap = 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();         /* OOM: terminate */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle the two known base cases directly. */
    if (n == 0) return 1;
    if (n == 1) return 3;

    /* Make sure the cache can hold up to index n. */
    ensure_cache(n);

    /* Initialize cache if it has not been grown yet. */
    if (cache_size == 0) {
        cache[0] = 1;
        cache[1] = 3;
        cache_size = 2;
    }

    /* Return cached value if already computed. */
    if (n < cache_size) return cache[n];

    /* Compute values from the last computed index up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = 8*a(i-1) - 4*a(i-2) */
        cache[i] = (seq_t)8 * cache[i - 1] - (seq_t)4 * cache[i - 2];
    }
    cache_size = (size_t)(n + 1);

    return cache[n];
}
