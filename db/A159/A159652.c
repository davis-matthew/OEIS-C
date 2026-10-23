/*
 * A159652  Numerator of Hermite(n, 14/19).
 * https://oeis.org/A159652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 28 * a(n-1) - 722 * (n-1) * a(n-2),
   a(0) = 1, a(1) = 28.   This computes the nth numerator of Hermite(n, 14/19). */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;

    /* Allocate and/or extend the cache. */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) abort();           /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)28;
    }

    if (cache_len <= n) {
        /* Reallocate to hold n+1 elements. */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) abort();             /* allocation failure */
        cache = tmp;

        /* Fill in the new terms using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t term = (seq_t)722 * ((seq_t)(i - 1));
            term = term * a2;
            cache[i] = (seq_t)28 * a1 - term;
        }
        cache_len = new_len;
    }

    return cache[n];
}
