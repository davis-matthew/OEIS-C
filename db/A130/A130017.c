/*
 * A130017  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+967)^2 = y^2.
 * https://oeis.org/A130017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 1934 for n > 6; initial values: a(1)
 *            = 0, a(2) = 45, a(3) = 2688, a(4) = 2901, a(5) = 3128, a(6) =
 *            18105
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 27, u64 n <= 65, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;        /* array of computed terms, unused index 0 */
static size_t cache_len = 0;       /* allocated length, includes index 0 */
static idx_t  computed_up_to = 0;  /* largest index for which cache is set */

/* Expand the cache so that it can hold index n (1‑based).  Fill new
   entries with initial values and compute any terms that become
   reachable by the recurrence. */
static void ensure_cache(idx_t n)
{
    if (n <= computed_up_to)        /* already computed */
        return;

    /* Determine the required array size: at least n+1, and at least 7
       so that indices 1…6 are present for the recurrence. */
    size_t required = (size_t)n + 1;
    if (required < 7)
        required = 7;

    if (cache == NULL) {
        cache = (seq_t *)malloc(required * sizeof(seq_t));
        if (cache == NULL) abort();        /* out of memory */
        cache_len = required;
        /* initialize all entries to 0 */
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = (seq_t)0;
        /* set the initial six terms */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)45;
        cache[3] = (seq_t)2688;
        cache[4] = (seq_t)2901;
        cache[5] = (seq_t)3128;
        cache[6] = (seq_t)18105;
        computed_up_to = 6;
    }

    if (required > cache_len) {
        /* Allocate a larger array */
        seq_t *new_cache = (seq_t *)malloc(required * sizeof(seq_t));
        if (new_cache == NULL) abort();
        /* Copy existing values */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        /* Zero any new slots (only needed for safety) */
        for (size_t i = cache_len; i < required; ++i)
            new_cache[i] = (seq_t)0;
        free(cache);
        cache = new_cache;
        cache_len = required;
        /* The initial values 1…6 were already copied; no need to reset. */
    }

    /* Compute new terms via the recurrence */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i > 6) {
            cache[i] = (seq_t)6 * cache[i-3] - cache[i-6] + (seq_t)1934;
        } /* else the value is already set by the initial block */
    }
    computed_up_to = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */
    ensure_cache(n);
    return cache[n];
}
