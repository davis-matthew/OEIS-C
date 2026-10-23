/*
 * A151946  a(1) = 8127; thereafter a(n) = (a(n-1) with digits sorted into
 *          descending order) - (a(n-1) with digits sorted into ascending
 *          order) (see the Kaprekar map, A151949).
 * https://oeis.org/A151946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   kaprekar_step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 8127; a(n) = kaprekar(a(n‑1)) for n > 1
   Kaprekar step: sort the decimal digits of x in descending order,
   sort them in ascending order, subtract: desc − asc.
   Handles arbitrary-width decimal representation (no leading zeros). */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/kaprekar_step.h"

/* internal cache for already computed terms */
static seq_t *cache   = NULL;   /* array of a(i) */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity of cache */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* out of range */

    /* Grow cache if needed */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
        if (!tmp) return 0;            /* allocation failure (unlikely) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first term if cache empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)8127;
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        cache[i] = kaprekar_step(cache[i - 1]);
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
