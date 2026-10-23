/*
 * A192777  Constant term in the reduction of the n-th Fibonacci polynomial by
 *          x^3->x^2+3x+1. See Comments.
 * https://oeis.org/A192777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+6*a(n-2)-a(n-3)-6*a(n-4)+a(n-5)+a(n-6) initial
 *            values: a(1)=1, a(2)=0, a(3)=1, a(4)=1, a(5)=2, a(6)=8
 *            Recurrence derived from OEIS entry A192777
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset (should not happen). */
    if (n == 0) return (seq_t)0;

    /* Initial sequence values.  a[0] corresponds to a(1). */
    static const seq_t init[6] = { (seq_t)1, (seq_t)0, (seq_t)1,
                                   (seq_t)1, (seq_t)2, (seq_t)8 };

    if (n <= 6)
        return init[n-1];

    /* Static cache to allow arbitrary order of calls. */
    static seq_t *cache = NULL;
    static idx_t cached_count = 0;   /* number of terms stored in cache */

    /* Allocate cache for the first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        cached_count = 6;
    }

    /* Extend cache if needed. */
    if (n > cached_count) {
        /* Reallocate to hold n terms. */
        cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        /* Compute missing terms using the recurrence. */
        for (idx_t k = cached_count + 1; k <= n; ++k) {
            /* k is 1‑based; cache indices are 0‑based. */
            seq_t term = cache[k-2]            /* a(k-1) */
                       + 6 * cache[k-3]        /* +6*a(k-2) */
                       - cache[k-4]            /* -a(k-3) */
                       - 6 * cache[k-5]        /* -6*a(k-4) */
                       + cache[k-6]            /* +a(k-5) */
                       + cache[k-7];           /* +a(k-6) */
            cache[k-1] = term;
        }
        cached_count = n;
    }

    return cache[n-1];
}
