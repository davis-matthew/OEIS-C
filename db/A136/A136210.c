/*
 * A136210  Numerators in continued fraction [0; 1, 3, 1, 3, 1, 3, ...].
 * https://oeis.org/A136210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(1)=1, a(2)=3; a(2n-1)=a(2n-2)+a(2n-3); a(2n)=3*a(2n-1)+a(2n-2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;   /* maximum index already computed */

    if (n < 1)                 /* out of range – never used by caller */
        return 0;

    /* Initialise cache on first use */
    if (!cache) {
        /* Allocate room for indices 0..2 inclusive (index 0 unused) */
        cache = (seq_t *)calloc(3, sizeof(seq_t));
        if (!cache) abort();   /* cannot happen in normal use */
        cache[0] = 0;          /* unused */
        cache[1] = 1;
        cache[2] = 3;
        cached_max = 2;
    }

    /* Extend the cache if required */
    if (n > cached_max) {
        size_t new_size = (size_t)n + 1;   /* to hold indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i & 1U) {   /* odd index: a(i)=a(i-1)+a(i-2) */
                cache[i] = cache[i-1] + cache[i-2];
            } else {        /* even index: a(i)=3*a(i-1)+a(i-2) */
                cache[i] = (cache[i-1] * 3) + cache[i-2];
            }
        }
        cached_max = n;
    }

    return cache[n];
}
