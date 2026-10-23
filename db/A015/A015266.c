/*
 * A015266  Gaussian binomial coefficient [ n,3 ] for q = -2.
 * https://oeis.org/A015266
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 24, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015266: Gaussian binomial coefficient [n,3] for q = -2.
 * Recurrence:   a(n) = -5*a(n-1) + 30*a(n-2) + 40*a(n-3) - 64*a(n-4)
 * Initial values:
 *   a(3) =  1
 *   a(4) = -5
 *   a(5) = 55
 *   a(6) = -385
 * The function stores computed values in a static cache, allowing
 * arbitrary order of calls for any n ≥ 3.  For n < 3 the function
 * returns 0 (the sequence is not defined there). */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;       /* cache[0..n] for computed values */
static size_t cache_cap = 0;      /* capacity of cache array */
static idx_t  cache_max = 2;      /* largest index that has been initialized */

/* Ensure cache has capacity for indices up to n.  Preserve existing data. */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_cache) return;   /* allocation failed; leave cache unchanged */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;   /* undefined for n < 3 */

    ensure_cache_capacity(n);

    /* Initialize base values if they have not yet been set. */
    if (cache_max < 3) {
        cache[3] = (seq_t)1;
        cache_max = 3;
        if (n >= 4) { cache[4] = (seq_t)-5; cache_max = 4; }
        if (n >= 5) { cache[5] = (seq_t)55;  cache_max = 5; }
        if (n >= 6) { cache[6] = (seq_t)-385; cache_max = 6; }
    }

    /* Compute any missing terms up to n using the recurrence. */
    if (cache_max < n) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            cache[i] = - (seq_t)5 * cache[i-1]
                      + (seq_t)30 * cache[i-2]
                      + (seq_t)40 * cache[i-3]
                      - (seq_t)64 * cache[i-4];
        }
        cache_max = n;
    }

    return cache[n];
}
