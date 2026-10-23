/*
 * A166471  a(n) = 2^L(n+1)*3^L(n), where L(n) is the n-th Lucas number
 *          (A000032(n)).
 * https://oeis.org/A166471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*a(n-2) (n > 1), a(0)=18, a(1)=24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A166471 */
seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated size of cache */
    static size_t size  = 0;      /* number of terms stored in cache */

    size_t idx = (size_t)n;

    /* Ensure the cache can hold the requested index */
    if (idx >= cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= idx) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first two terms once */
    if (size < 2) {
        cache[0] = (seq_t)18;
        cache[1] = (seq_t)24;
        size = 2;
    }

    /* Compute any missing terms up to n */
    for (size_t i = size; i <= idx; ++i) {
        if (i < 2) continue;           /* first two terms already set */
        cache[i] = cache[i-1] * cache[i-2];
    }

    /* Update the count of stored values */
    if (size <= idx) size = idx + 1;

    return cache[idx];
}
