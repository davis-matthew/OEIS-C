/*
 * A035278  One ninth of deca-factorial numbers.
 * https://oeis.org/A035278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) * (10*n - 1); a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A035278: One ninth of deca-factorial numbers. */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* cache[0] unused, cache[1] = a(1) */
    static size_t cache_len = 0; /* next uncomputed index (i.e., cache_len == last computed index + 1) */

    if (n < 1)
        return 0; /* out of offset – undefined, but return 0 */

    /* Ensure cache is allocated up to n */
    if (!cache) {
        /* Allocate for indices 0..n */
        cache_len = n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0; /* malloc failure; caller must handle */
        cache[0] = 0;        /* unused */
        if (n >= 1) {
            cache[1] = 1;    /* a(1) = 1 */
        }
    } else if ((size_t)n + 1 > cache_len) {
        /* Extend the cache to hold up to n */
        size_t old_len = cache_len;
        cache_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!new_cache) return 0; /* realloc failure */
        cache = new_cache;
        /* Compute values from old_len to n inclusive */
        for (size_t i = old_len; i <= (size_t)n; ++i) {
            if (i == 1) {
                cache[1] = 1;
                continue;
            }
            seq_t factor = (seq_t)((seq_t)10 * (seq_t)i - 1);
            cache[i] = cache[i - 1] * factor;
        }
    } else if ((size_t)n + 1 > cache_len) {
        /* Compute missing values without resizing (should not happen) */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t factor = (seq_t)((seq_t)10 * (seq_t)i - 1);
            cache[i] = cache[i - 1] * factor;
        }
        cache_len = n + 1;
    } else if ((size_t)n + 1 > cache_len) {
        /* Already covered above; no action needed */
    }

    /* Ensure the value for n is computed */
    if ((size_t)n + 1 > cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t factor = (seq_t)((seq_t)10 * (seq_t)i - 1);
            cache[i] = cache[i - 1] * factor;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
