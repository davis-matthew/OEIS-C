/*
 * A160193  Numerator of Hermite(n, 5/28).
 * https://oeis.org/A160193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 5*a(n-1) - 392*(n-1)*a(n-2), a(0)=1, a(1)=5
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The following static array caches computed values up to the maximum
   index requested so far.  It grows only once per new maximum.  */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of computed entries (cache_len = current maximum index + 1) */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure that the cache has room for at least 'needed' entries. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)5;

    /* Initialise the cache on first use. */
    if (cache == NULL) {
        cache_cap = 4;               /* start with a small capacity   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache_len = 2;                /* indices 0 and 1 are filled   */
    }

    /* If we have not yet computed up to the requested index,
       compute the missing values with the recurrence. */
    if (cache_len <= n) {
        ensure_capacity(n + 1);       /* make sure there is a slot   */
        for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
            seq_t term1 = (seq_t)5 * cache[k - 1];
            seq_t term2 = (seq_t)392 * ((seq_t)(k - 1)) * cache[k - 2];
            cache[k] = term1 - term2;
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
