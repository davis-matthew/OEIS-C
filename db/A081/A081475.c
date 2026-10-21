/*
 * A081475  Consider the mapping f(x/y) = (x+y)/(2xy) where x/y is a reduced
 *          fraction. Beginning with x_0 = 1 and y_0 = 2, repeated application
 *          of this mapping produces a sequence of fractions x_n/y_n; a(n) is
 *          the n-th numerator.
 * https://oeis.org/A081475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0)=1, a(1)=3, a(n) = a(n-1) + 2*a(n-2)*(a(n-1)-a(n-2)) for n
 *            >= 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache   = NULL;
    static size_t computed = 0;   /* number of terms stored (valid indices 0..computed-1) */
    static size_t capacity = 0;   /* allocated size of cache array */

    size_t idx = (size_t)n;

    /* Initial allocation on first call */
    if (cache == NULL) {
        capacity = (idx + 1 < 2 ? 2 : idx + 1);
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* malloc failure fallback */
        cache[0] = (seq_t)1;
        if (capacity > 1) cache[1] = (seq_t)3;
        computed = (capacity > 1 ? 2 : 1);
    }

    /* Grow capacity if needed (but keep at least 2) */
    if (idx >= capacity) {
        size_t new_capacity = (idx + 1 < 2 ? 2 : idx + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        capacity = new_capacity;
    }

    /* Compute missing terms up to idx */
    if (idx >= computed) {
        for (size_t i = computed; i <= idx; ++i) {
            /* For i=0 or i=1, cached values already exist */
            if (i == 0) continue;
            if (i == 1) continue;
            seq_t a_prev2 = cache[i - 2];
            seq_t a_prev1 = cache[i - 1];
            seq_t diff   = a_prev1 - a_prev2;          /* a(i-1) - a(i-2) */
            seq_t term   = (seq_t)2 * a_prev2 * diff;   /* 2 * a(i-2) * diff */
            cache[i] = a_prev1 + term;                 /* a(i) */
        }
        computed = idx + 1;
    }

    return cache[idx];
}
