/*
 * A110512  Expansion of (1 + x)/(1 + x + 2x^2).
 * https://oeis.org/A110512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n+2) + a(n+1) + 2*a(n) = 0 Initial values: a(0) = 1, a(1) = 0
 *            (This satisfies a(2) = -2, a(3) = 2, etc.)
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 61, i64 n <= 126,
 *            i128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build environment. */
extern seq_t a(idx_t n); /* declared below */

static seq_t *cache = NULL;   /* array of computed a(i) */
static idx_t max_index = 0;   /* largest index currently stored in cache */
static bool initialized = false;  /* whether cache has been initialized */

/* Return the value a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* Lazy initialization: first call creates cache with a(0) and a(1) */
    if (!initialized) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        max_index = 1;
        initialized = true;
    }

    /* If we already have the result, return it immediately */
    if (n <= max_index)
        return cache[n];

    /* Need to extend cache up to index n */
    idx_t required = n + 1;                    /* number of entries needed */
    idx_t new_size = required > max_index * 2 ? required : max_index * 2;
    if (new_size < required) new_size = required;  /* safety check */

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;           /* allocation failed */
    cache = new_cache;

    /* Compute new entries using the linear recurrence */
    for (idx_t i = max_index + 1; i <= n; ++i) {
        /* a(i) = -(a(i-1) + 2*a(i-2)) */
        cache[i] = -(cache[i - 1] + ((seq_t)2 * cache[i - 2]));
    }

    max_index = n;   /* update last computed index */
    return cache[n];
}
