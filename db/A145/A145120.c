/*
 * A145120  Numbers X such that (X^2-19)/57 is a square.
 * https://oeis.org/A145120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 302*a(n-1) - a(n-2) with a(1) = 38, a(2) = 11438
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* array holding terms 1.. */
static size_t cache_cap = 0;   /* allocated number of slots */
static idx_t cache_max = 0;    /* highest index computed */

/* grow the cache so that it can store indices up to 'need' (inclusive) */
static void ensure_cache(size_t need)
{
    if (cache_cap > need)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 4;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* no memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise the first two known values of the sequence */
static void init_initial(void)
{
    if (cache_cap < 3)
        ensure_cache(2);
    cache[1] = (seq_t)38;
    cache[2] = (seq_t)11438;
    cache_max = 2;
}

/* return the n-th term (OEIS indexing: a(1) is the first term) */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    /* make sure we have storage and initial values */
    ensure_cache((size_t)n);
    if (cache_max < 2)
        init_initial();

    /* if we already know the term, return it */
    if (n <= cache_max)
        return cache[(size_t)n];

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        /* recurrence: a(i) = 302*a(i-1) - a(i-2) */
        cache[(size_t)i] = (seq_t)(302 * cache[(size_t)(i-1)] - cache[(size_t)(i-2)]);
    }
    cache_max = n;
    return cache[(size_t)n];
}
