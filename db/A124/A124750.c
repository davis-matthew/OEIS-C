/*
 * A124750  Expansion of (1 + x + x^2)/(1 - x^3 + x^4).
 * https://oeis.org/A124750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-3) - a(n-4) initial values: a(0)=1, a(1)=1, a(2)=1,
 *            a(3)=1, a(4)=0, a(5)=0, a(6)=0
 * Range:     i8 n <= 40, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic cache of computed values */
static size_t cache_len = 0;     /* number of values stored (0..cache_len-1) */
static size_t cache_cap = 0;     /* current allocation capacity */

/* Initialise the cache on the first call.  */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_cap = 7;          /* store values a(0)..a(6) */
        cache_len = 7;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();   /* allocation failure */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 0;
        cache[5] = 0;
        cache[6] = 0;
    }
}

seq_t a(idx_t n)
{
    /* Ensure the cache is ready. */
    init_cache();

    /* Small n handled by the cache directly. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache if necessary. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 7;
        while (new_cap < needed)
            new_cap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new values up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* For i >= 7 the recurrence applies. For the first few i
           the values are copied from the pre‑initialised cache. */
        if (i >= 7)
            cache[i] = cache[i - 3] - cache[i - 4];
        /* i < 7 already holds the correct value from the base case. */
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
