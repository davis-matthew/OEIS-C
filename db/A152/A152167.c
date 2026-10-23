/*
 * A152167  a(n)=-a(n-1)+3*a(n-2), n>1 ; a(0)=1, a(1)=-3 .
 * https://oeis.org/A152167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 3*a(n-2); a(0) = 1, a(1) = -3
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 25, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* dynamic array of computed values */
static size_t cache_size = 0;     /* number of values that are computed */
static size_t cache_cap  = 0;     /* allocated capacity of the array */

/* Grow the cache to at least hold values up to index 'up_to'.
 * Compute any new terms using the recurrence. */
static void ensure_cache(idx_t up_to)
{
    /* No need to grow if enough space already exists */
    if (up_to < cache_size)
        return;

    /* Determine new capacity (check for overflow trivial in 64‑bit range) */
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (up_to >= new_cap)
        new_cap *= 2;

    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1);                     /* simple fatal error handling */
    seq_cache = new_arr;
    cache_cap = new_cap;

    /* Compute any missing values */
    for (idx_t i = cache_size; i <= up_to; ++i) {
        if (i == 0) {
            seq_cache[0] = (seq_t)1;
        } else if (i == 1) {
            seq_cache[1] = (seq_t)-3;
        } else {
            seq_cache[i] = -seq_cache[i-1] + (seq_t)3 * seq_cache[i-2];
        }
    }
    cache_size = up_to + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
