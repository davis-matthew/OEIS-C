/*
 * A153122  G.f.: 1/p(x) where p(x)=x^5 + 2x^4 - 2x^3 - x^2 + 2x - 1.
 * https://oeis.org/A153122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A153122: a(n) = -2*a(n-1) + 2*a(n-2) + a(n-3) - 2*a(n-4) + a(n-5),
   a(0)=1, a(1)=-2, a(2)=6, a(3)=-15, a(4)=38 */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_computed = 0;

    /* initialize cache on first call */
    if (cache == NULL) {
        capacity = (size_t)n + 1;
        cache = alloc_seq_array(capacity);
        cache[0] = (seq_t)1;
        max_computed = 0;
    } else if ((size_t)n + 1 > capacity) { /* grow if needed */
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (size_t i = 0; i <= max_computed; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        capacity = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 1) val += -2 * cache[i - 1];
        if (i >= 2) val +=  2 * cache[i - 2];
        if (i >= 3) val +=  1 * cache[i - 3];
        if (i >= 4) val += -2 * cache[i - 4];
        if (i >= 5) val +=  1 * cache[i - 5];
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
