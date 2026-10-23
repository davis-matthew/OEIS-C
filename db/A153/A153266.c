/*
 * A153266  a(n) = -4*a(n-3) + 11*a(n-2) - a(n-1), a(0) = 13, a(1) = -19, a(2)
 *          = 162.
 * https://oeis.org/A153266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 5, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -4*a(n-3) + 11*a(n-2) - a(n-1)
   Initial values: a(0) = 13, a(1) = -19, a(2) = 162 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoization array */
static idx_t cache_len = 0;        /* allocated length of cache */
static idx_t computed_max = (idx_t)-1;   /* highest n already computed */

/* Ensure that cache has space for indices up to (and including) n */
static void ensure_cache(idx_t n)
{
    idx_t required = (n < 2) ? 3 : n + 1;
    if (cache_len >= required)
        return;

    seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
    if (!tmp)  /* allocation failed – keep old cache */
        return;
    cache = tmp;
    cache_len = required;

    if (computed_max == (idx_t)-1) {          /* first allocation: set seeds */
        cache[0] = 13;
        cache[1] = -19;
        cache[2] = 162;
        computed_max = 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 13;
    if (n == 1) return -19;
    if (n == 2) return 162;

    ensure_cache(n);

    /* If we already have the value, just return it */
    if (computed_max >= n)
        return cache[n];

    /* Compute all missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = -4 * cache[i - 3] + 11 * cache[i - 2] - cache[i - 1];
    }
    computed_max = n;

    return cache[n];
}
