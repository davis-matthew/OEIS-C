/*
 * A037796  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,1,0.
 * https://oeis.org/A037796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; for n≥2, a(n)=7*a(n-1)+d(n) where d(n)=3,2,1,0 repeating
   (equivalent to a(n)=7*a(n-1)+a(n-4)-7*a(n-5)). */
#include <stdlib.h>

static seq_t *cache = NULL;       /* 1‑based cache of computed terms */
static idx_t cache_size = 0;      /* number of terms stored */

/* Return the n‑th term of OEIS A037796, n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* behaviour below the offset is irrelevant */
        return (seq_t)0;
    }

    /* If already computed, return it. */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Resize the cache to hold terms up to n. */
    idx_t new_size = n;

    seq_t *new_cache = NULL;
#ifdef __cplusplus
    new_cache = new seq_t[new_size];
#else
    new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
#endif
    if (!new_cache) {
        abort();
    }

    /* Copy existing values. */
    for (idx_t i = 0; i < cache_size; ++i) {
        new_cache[i] = cache[i];
    }

#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache = new_cache;

    /* Initialize first term if necessary. */
    if (cache_size == 0) {
        cache[0] = (seq_t)3;   /* a(1) = 3 */
        cache_size = 1;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        seq_t digit;
        switch (i % 4) {
            case 0: digit = (seq_t)3; break;
            case 1: digit = (seq_t)2; break;
            case 2: digit = (seq_t)1; break;
            default: digit = (seq_t)0; break;
        }
        cache[i] = cache[i - 1] * (seq_t)7 + digit;
    }

    cache_size = new_size;
    return cache[n - 1];
}
