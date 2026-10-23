/*
 * A160312  Numerator of Hermite(n, 14/31).
 * https://oeis.org/A160312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 31^n * Hermite(n, 14/31) Recurrence: a(0)=1, a(1)=28
 *            a(n+2) = 28*a(n+1) - 1922*(n+1)*a(n) for n>=0
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* current allocated capacity */
    static size_t max_computed = 0;       /* number of values already computed (cache[0..max_computed-1]) */

    /* Allocate minimal initial capacity if needed */
    if (cache == NULL) {
        grow_seq(&cache, &cache_cap, (size_t)2);   /* at least two slots for a(0) and a(1) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)28;
        max_computed = 2;
    }

    /* Ensure capacity for requested index */
    if ((size_t)n + 1 > cache_cap)
        grow_seq(&cache, &cache_cap, (size_t)n + 1);

    /* Compute any missing terms up to n */
    for (size_t i = max_computed; i <= (size_t)n; ++i) {
        if (i == 0) {            /* already set */
            continue;
        } else if (i == 1) {     /* already set */
            continue;
        } else {
            /* a(i) = 28*a(i-1) - 1922*(i-1)*a(i-2) */
            seq_t prev = cache[i - 1];
            seq_t prevprev = cache[i - 2];
            seq_t mul = (seq_t)(i - 1);
            cache[i] = (seq_t)28 * prev - (seq_t)1922 * mul * prevprev;
        }
    }
    max_computed = (size_t)n + 1;
    return cache[(size_t)n];
}
