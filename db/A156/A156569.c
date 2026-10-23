/*
 * A156569  a(n) = 6*a(n-1)-a(n-2) for n > 2; a(1)=37, a(2)=205.
 * https://oeis.org/A156569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) for n > 2, a(1)=37, a(2)=205
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoises previously computed values. */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Return 0 for an invalid (zero) index, although it should never be called. */
    if (n == 0) return 0;

    /* Static cache initialised on first call. */
    static seq_t *cache = NULL;
    static size_t len = 0;   /* number of terms stored in cache */
    static size_t cap = 0;   /* allocated capacity of cache */

    if (cache == NULL) {
        cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;       /* out of memory – abort */
        cache[0] = (seq_t)37;             /* a(1) */
        cache[1] = (seq_t)205;            /* a(2) */
        len = 2;
    }

    /* If the requested term is already cached, return it immediately. */
    if ((size_t)n <= len) {
        return cache[n - 1];
    }

    /* Resize cache ahead of time if necessary. */
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure – abort */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (size_t i = len; i < (size_t)n; ++i) {
        cache[i] = 6 * cache[i - 1] - cache[i - 2];
    }

    len = (size_t)n;
    return cache[n - 1];
}
