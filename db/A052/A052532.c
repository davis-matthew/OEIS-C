/*
 * A052532  Expansion of (1-x)/(1-x-x^3-x^4+x^5).
 * https://oeis.org/A052532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) - a(n-5); Initial values:
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=2
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 56, u64 n <= 108,
 *            u128 n <= 213, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

/* Cached values of the sequence.  */
static seq_t *cache = NULL;
static size_t  cache_cap = 0;   /* allocated elements */
static size_t  max_computed = 0;/* largest index that has a value */

/* Allocate or grow the cache so that it can hold indices up to n.  */
static void ensure_cache_size(idx_t n)
{
    if (n < (idx_t)cache_cap)
        return;

    size_t new_cap = cache_cap < 8 ? 8 : cache_cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;          /* double until enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Seed the first five terms.  */
static void init_seed(void)
{
    ensure_cache_size(4);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)1;
    cache[4] = (seq_t)2;
    max_computed = 4;
}

/* Return a(n) for any n >= 0.  */
seq_t a(idx_t n)
{
    if (cache == NULL)
        init_seed();

    if (n <= (idx_t)max_computed)
        return cache[(size_t)n];

    ensure_cache_size(n);

    /* Compute new terms up to n.  */
    for (idx_t i = (idx_t)(max_computed + 1); i <= n; ++i) {
        cache[(size_t)i] =
            cache[(size_t)(i - 1)] +
            cache[(size_t)(i - 3)] +
            cache[(size_t)(i - 4)] -
            cache[(size_t)(i - 5)];
    }
    max_computed = n;
    return cache[(size_t)n];
}
