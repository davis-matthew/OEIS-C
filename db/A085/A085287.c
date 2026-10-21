/*
 * A085287  Expansion of g.f. (1+4*x)/((1-x)*(1+x)*(1-3*x)).
 * https://oeis.org/A085287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-10 - 3*(-1)^n + 21*3^n)/8
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7, a(n)=2*a(n-1)+3*a(n-2)+5 for n≥2 */

/* This implementation caches computed values in a static array,
   extending it as needed. It works for all supported seq_t types,
   including arbitrary‑precision integer classes. */
#include <stdlib.h>

static seq_t *cache = NULL;       /* cached terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of valid terms stored */

/* Ensure the cache can hold at least "needed" entries. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                 /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialize the cache with the first two terms. */
static void init_cache(void)
{
    ensure_capacity(2);
    cache[0] = (seq_t)1;          /* a(0) */
    cache[1] = (seq_t)7;          /* a(1) */
    cache_len = 2;
}

seq_t a(idx_t n)
{
    /* Lazy initialization. */
    if (cache_len == 0)
        init_cache();

    /* Extend the cache up to n if necessary. */
    if ((size_t)n >= cache_len) {
        ensure_capacity((size_t)n + 1);
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            /* a(i) = 2*a(i-1) + 3*a(i-2) + 5 */
            cache[i] = (seq_t)((seq_t)2 * cache[i - 1]
                              + (seq_t)3 * cache[i - 2]
                              + (seq_t)5);
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
