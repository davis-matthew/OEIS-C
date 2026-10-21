/*
 * A097831  Partial sums of Chebyshev sequence S(n,17) = U(n,17/2) =
 *          A078366(n).
 * https://oeis.org/A097831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - a(n-2) + 1, a(0) = 1, a(1) = 18, for n >= 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t size = 0, cap = 0;

    /* initialise on first call */
    if (!cache) {
        cap = 2;                     /* at least two entries needed */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;         /* a(0) */
        cache[1] = (seq_t)18;        /* a(1) */
        size = 2;
    }

    /* extend the cache up to the requested index */
    if (n >= size) {
        for (idx_t i = size; i <= n; ++i) {
            /* grow capacity if necessary */
            if (i >= cap) {
                idx_t newcap = cap * 2;
                if (newcap <= i) newcap = i + 1;  /* safety for very large n */
                cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                cap = newcap;
            }

            /* recurrence: a(n) = 17*a(n-1) - a(n-2) + 1 */
            if (i == 0) {
                cache[i] = (seq_t)1;     /* already set, harmless */
            } else if (i == 1) {
                cache[i] = (seq_t)18;    /* already set, harmless */
            } else {
                cache[i] = (seq_t)17 * cache[i-1] - cache[i-2] + (seq_t)1;
            }
        }
        size = n + 1;
    }

    return cache[n];
}
