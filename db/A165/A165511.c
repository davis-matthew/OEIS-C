/*
 * A165511  a(0)=1, a(1)=10, a(n) = 90*a(n-2) - a(n-1).
 * https://oeis.org/A165511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=10, a(n)=90*a(n-2)-a(n-1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 20, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Computes the n‑th term of OEIS sequence A165511.  It memoizes already
 * computed values so that any order of calls works fast.  The function
 * works with the generic integer type seq_t, which may be a built‑in
 * integral type or an arbitrary‑precision class. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static idx_t   cache_len = 0; /* number of terms stored (next free index) */
    static size_t  cache_cap = 0; /* capacity of the array */

    /* Ensure capacity for indices 0..n and at least two slots for the
     * initial values. */
    size_t needed = (size_t)(n + 1);
    if (needed < 2) needed = 2;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = needed;
    }

    /* Initialise base terms if they are not yet present. */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }
    if (cache_len < 2) {
        cache[1] = 10;
        cache_len = 2;
    }

    /* Compute any missing terms up to n. */
    idx_t start = cache_len;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)90 * cache[i - 2] - cache[i - 1];
    }

    /* Update the memoised length if we extended the cache. */
    if (n >= start) cache_len = n + 1;

    return cache[n];
}
