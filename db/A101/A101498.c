/*
 * A101498  Expansion of (1-x^2)/(1-3x+x^2+3x^3+x^4).
 * https://oeis.org/A101498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) - a(n-4), initial values:
 *            a(0)=1, a(1)=3, a(2)=7, a(3)=15
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 28, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* dynamic array helpers -------------------------------------------------- */
static seq_t *resize_array(seq_t *old, idx_t old_size, idx_t new_size)
{
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_size]();        /* value–init for C++ objects   */
#else
    new_arr = (seq_t *)calloc(new_size, sizeof(seq_t));   /* zero for C */
#endif
    /* copy old data */
    idx_t i;
    for (i = 0; i < old_size && i < new_size; ++i)
        new_arr[i] = old[i];

    /* free old storage */
    if (old) {
#ifdef __cplusplus
        delete[] old;
#else
        free(old);
#endif
    }
    return new_arr;
}

static seq_t *cache = NULL;        /* array of already computed terms */
static idx_t cache_size = 0;       /* current size of the cache */

/* Computes the n-th term of OEIS sequence A101498. */
seq_t a(idx_t n) {
    /* Return the cached value if we have already computed it. */
    if (n < cache_size)
        return cache[n];

    /* Ensure the cache is large enough. */
    idx_t new_size = n + 1;
    cache = (seq_t *)resize_array(cache, cache_size, new_size);
    /* Compute all missing terms up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;
        else if (i == 1)
            val = (seq_t)3;
        else if (i == 2)
            val = (seq_t)7;
        else if (i == 3)
            val = (seq_t)15;
        else {
            val = (seq_t)3 * cache[i - 1];   /* 3*a(n-1) */
            val -= cache[i - 2];             /* -a(n-2) */
            val -= (seq_t)3 * cache[i - 3];   /* -3*a(n-3) */
            val -= cache[i - 4];             /* -a(n-4) */
        }
        cache[i] = val;
    }
    cache_size = new_size;
    return cache[n];
}
