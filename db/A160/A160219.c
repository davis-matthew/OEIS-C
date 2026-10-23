/*
 * A160219  Numerator of Hermite(n, 17/28).
 * https://oeis.org/A160219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 14^n * Hermite(n, 17/28) Recurrence (derived from
 *            Hermite): a(0)=1, a(1)=17, a(n) = 17*a(n-1) - 392*(n-1)*a(n-2)
 *            for n >= 2
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored (0 means none) */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* Ensure cache can hold up to n */
    if (n >= cache_cap) {
        /* choose new capacity: at least n+1, and at least double if possible */
        idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill in missing values using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)17;
        } else {
            seq_t term1 = cache[i-1] * (seq_t)17;
            seq_t factor = (seq_t)392 * (seq_t)(i-1);
            seq_t term2 = cache[i-2] * factor;
            cache[i] = term1 - term2;
        }
    }
    cache_size = n + 1;   /* we have computed up to index n */

    return cache[n];
}
