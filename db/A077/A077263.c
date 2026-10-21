/*
 * A077263  Number of (undirected) cycles in the n-th order antiprism graph.
 * https://oeis.org/A077263
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 8*a(n-3) - 3*a(n-4) + 2*a(n-5) -
 *            a(n-6) (n > 8) Initial values: a(3)=63, a(4)=179, a(5)=523,
 *            a(6)=1619, a(7)=5239, a(8)=17379
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 3.  For n < 3 we return 0. */
    if (n < 3) return (seq_t)0;

    /* Lazy allocation of cache.  We keep values up to the largest
       index that has been computed so far. */
    static seq_t *cache = NULL;
    static idx_t cached_to = 2;   /* no values computed yet */

    /* Ensure enough space for the first few values and set them. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(9 * sizeof(seq_t));   /* indices 0…8 */
        for (idx_t i = 0; i < 9; ++i) cache[i] = (seq_t)0;
        cache[3] = (seq_t)63;
        cache[4] = (seq_t)179;
        cache[5] = (seq_t)523;
        cache[6] = (seq_t)1619;
        cache[7] = (seq_t)5239;
        cache[8] = (seq_t)17379;
        cached_to = 8;
    }
    /* Expand cache if the caller asks for a larger index. */
    if (n > cached_to) {
        if ((n + 1) > (cached_to + 1)) {
            cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        }
        /* Compute new values using the recurrence. */
        for (idx_t k = cached_to + 1; k <= n; ++k) {
            if (k <= 8) continue;                     /* base values already set */
            /* Compute the positive and negative parts separately
               to avoid underflow when cache elements are unsigned. */
            seq_t pos = ((seq_t)6 * cache[k-1] +
                         (seq_t)8 * cache[k-3] +
                         (seq_t)2 * cache[k-5]);

            seq_t neg = ((seq_t)11 * cache[k-2] +
                         (seq_t)3 * cache[k-4] +
                         cache[k-6]);

            cache[k] = pos - neg;
        }
        cached_to = n;
    }
    return cache[n];
}
